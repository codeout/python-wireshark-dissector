Vagrant.configure(2) do |config|
  config.vm.box = 'ubuntu/bionic64'
  config.vm.provider :virtualbox do |v|
    v.cpus   = 2
    v.memory = 2048
  end

  config.vm.provision 'shell', inline: <<-EOS
    sudo apt-get update
    sudo apt-get install -y build-essential pkg-config python3 python3-dev python3-pip  libglib2.0-dev libwireshark-dev libwiretap-dev
    pip3 install cython
  EOS

  config.vm.synced_folder "./", "/tmp/wireshark"
end
