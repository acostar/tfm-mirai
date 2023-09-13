@ECHO OFF
ECHO Starting mirai cnc and loader...

vagrant ssh mirai -c "sudo /vagrant/configs/start_2.sh"
PAUSE
