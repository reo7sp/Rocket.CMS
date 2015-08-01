Vagrant.configure("2") do |c|
	c.vm.box = "ubuntu/trusty64"
	c.vm.network "forwarded_port", guest: 23307, host: 23307
	c.vm.network "private_network", ip: "192.168.233.7"
	c.vm.synced_folder ".", "/vagrant/app"
	c.vm.provision "docker" do |p|
		p.build_image "/vagrant/app", args: "-t reo7sp/rocketcms"
		p.run "reo7sp/rocketcms", ports: ["23307:23307"]
	end
end
