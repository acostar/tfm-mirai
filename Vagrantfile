# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|

    config.vm.define "mirai" do |mirai|
      mirai.vm.box = "ubuntu/xenial64"
      mirai.vm.network "private_network", ip: "192.168.50.11"
      mirai.vm.provision "shell", path: "configs/provision.sh"
      mirai.vm.hostname = "mirai"

      mirai.vm.provider "virtualbox" do |vb|
        vb.name = "mirai"
        vb.memory = "2048"
        vb.cpus = 2
        vb.customize ["modifyvm", :id, "--uartmode1", "disconnected"]
      end
    end

    config.vm.define "miraiTelnet" do |miraiTelnet|
      miraiTelnet.vm.box = "ubuntu/xenial64"
      miraiTelnet.vm.network "private_network", ip: "192.168.50.12"
      miraiTelnet.vm.hostname = "miraiTelnet"

      miraiTelnet.vm.provider "virtualbox" do |vb|
        vb.name = "miraiTelnet"
        vb.memory = "2048"
        vb.cpus = 2
        vb.customize ["modifyvm", :id, "--uartmode1", "disconnected"]
      end
    end

    config.vm.define "miraiBot" do |miraiBot|
      miraiBot.vm.box = "ubuntu/xenial64"
      miraiBot.vm.network "private_network", ip: "192.168.50.13"
      # miraiBot.vm.synced_folder './', '/vagrant', disabled: true
      miraiBot.vm.provision "shell", path: "configs/provision_target.sh"
      # miraiBot.ssh.username = 'admin'
      # miraiBot.ssh.password = 'admin'
      # miraiBot.ssh.insert_key = true
      # miraiBot.ssh.shell = "sh"
      miraiBot.vm.hostname = "miraiBot"

      miraiBot.vm.provider "virtualbox" do |vb|
        vb.name = "miraiBot"
        vb.memory = "2048"
        vb.cpus = 2
        # vb.gui = false
      end
    end

    # (1..10).each do |i|
    #   config.vm.define "target_#{i}" do |target|
    #     target.vm.box = "cylab/tiny-core"
    #     target.vm.box_version = "20190627.221700.0"
    #     target.vm.box_check_update = false
    #     target.vm.network "private_network", ip: "192.168.50.#{100+i}"
    #     target.ssh.shell = "sh"
    #     target.vm.synced_folder './', '/vagrant', disabled: true
    #     target.vm.hostname = "target-#{i}"
    #     target.vm.provision "shell", path: "configs/provision_target.sh"
      
    #   target.vm.provider "virtualbox" do |vb|
    #     vb.name = "target_#{i}"
    #     vb.memory = "256"
    #     vb.cpus = 1
    #     vb.gui = true
    #   end
    #   end
    # end

end