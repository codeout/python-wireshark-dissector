#include <stdio.h>
#include <stdint.h>

#include <config.h>

// rawshark
#include <cfile.h>
#include <epan/column.h>
#include <epan/epan.h>
#include <epan/epan-int.h>
#include <file.h>
#include <frame_tvbuff.h>
#include <wiretap/pcap-encap.h>
#include <wsutil/privileges.h>

#include <cfile.c>
#include <frame_tvbuff.c>

capture_file cfile;

static const frame_data *ref = NULL;
static frame_data ref_frame;
static frame_data *prev_dis = NULL;
static frame_data prev_dis_frame;
static frame_data *prev_cap = NULL;
static frame_data prev_cap_frame;

static const nstime_t *
raw_get_frame_ts(void *data _U_, guint32 frame_num) {
    if (ref && ref->num == frame_num)
        return &ref->abs_ts;

    if (prev_dis && prev_dis->num == frame_num)
        return &prev_dis->abs_ts;

    if (prev_cap && prev_cap->num == frame_num)
        return &prev_cap->abs_ts;

    return NULL;
}

void rawshark_init(void) {
    e_prefs *prefs_p;

    // Avoid "assertion failed: (init_process_policies_called)"
    init_process_policies();

    // required for epan_init
    wtap_init();

    epan_init(register_all_protocols, register_all_protocol_handoffs, NULL, NULL);

    cap_file_init(&cfile);
    cfile.count = 0;

    prefs_p = epan_load_settings();
    build_column_format_array(&cfile.cinfo, prefs_p->num_cols, TRUE);
}

void rawshark_clean(void) {
    epan_cleanup();
    wtap_cleanup();
}

void rawshark_process_packet(uint8_t *data, int len, FILE *file) {
    guint32 cum_bytes = 0;
    gint64 data_offset = 0;
    frame_data fdata;
    epan_dissect_t *edt;

    // initialize epan
    cfile.epan = epan_new();
    cfile.epan->get_frame_ts = raw_get_frame_ts;
    cfile.epan->get_interface_name = NULL;
    cfile.epan->get_interface_description = NULL;

    edt = epan_dissect_new(cfile.epan, TRUE, TRUE);

    // initialize frame
    struct wtap_pkthdr *whdr = g_malloc(sizeof(struct wtap_pkthdr));

    whdr->rec_type = REC_TYPE_PACKET;
    whdr->pkt_encap = wtap_pcap_encap_to_wtap_encap(1); /* ETHERNET */
    whdr->caplen = len;
    whdr->len = whdr->caplen;
    whdr->opt_comment = NULL;

    cfile.count++;

    frame_data_init(&fdata, cfile.count, whdr, data_offset, cum_bytes);
    frame_data_set_before_dissect(&fdata, &cfile.elapsed_time, &ref, prev_dis);

    if (ref == &fdata) {
        ref_frame = fdata;
        ref = &ref_frame;
    }

    // dissect
    epan_dissect_run(edt, cfile.cd_t, whdr, frame_tvbuff_new(&fdata, data), &fdata, &cfile.cinfo);

    // clean and print
    frame_data_destroy(&fdata);
    g_free(whdr);

    frame_data_set_after_dissect(&fdata, &cum_bytes);
    prev_dis_frame = fdata;
    prev_dis = &prev_dis_frame;

    prev_cap_frame = fdata;
    prev_cap = &prev_cap_frame;

    write_json_proto_tree(NULL, print_dissections_expanded,
                          TRUE, NULL, PF_NONE,
                          edt, file);

    epan_dissect_free(edt);
    edt = NULL;

    epan_free(cfile.epan);
}
