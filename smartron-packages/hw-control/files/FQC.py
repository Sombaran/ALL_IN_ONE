import subprocess
import csv
import datetime
import re
from subprocess import Popen

testcase_list = ["Check PING Responce",
                 "Read_Device_SN",
                 "Read_And_Check_default_wifi_name",
                 "Read_And_Check_default_BT_name",
                 "Check etho operation state",
                 "Check  eth1 operation state",
                 "Read_And_Check_Zwave_ttyACM0_port",
                 "Read_And_Check_Zigbee_ttyHS0_port",
                 "Read_And_Check_MCU_fw_Version",
                 "TRONX_URL",
                 "Read_And_Check_DSP_fw_Version",
                 "Read_And_Check_fw_Build_Number",
                 "Read_And _Check_Firmware_version",
                 "Read_And_Check_BT_stack_Version",
                 "Read_And_Check_pulseaudio_Version",
                 "Read_And_Check_Shifu_logger_level",
                 "Read_Device_ath1_MAC",
                 "Read_Device_ath0_MAC",
                 "Read_Device_BT_MAC",
                 "Check ath0 WPS status",
                 "Check ath1 WPS status",
                 "DEVICE_REGISTRATION_STATUS",
                 "ALEXA_REGISTRATION_STATUS",
                 "Checking rtc",
                 "Checking hwclock"]

cmd_list1=["ping 8.8.8.8 -c5",
          "fw_printenv DEVICE_ID",
          "uci show wireless.@wifi-iface[0].ssid",
          "hciconfig -a | awk '/^[a-z]/ {name=$NF}/Name:/{print $2}'",
          "cat /sys/class/net/eth0/operstate",
          "cat /sys/class/net/eth1/operstate",
          "ls /dev/ttyACM0",
          "ls /dev/ttyHS0",
          "gcontrol mcu get version",
          "uci get shifu.conf.TRONX_URL | cut -d= -f2 | awk '{print $1}'",
          "cxdish fw-version | cut -d: -f2 | awk '{print $1}'",
          "lsb_release -b",
          "cat /etc/shifu_version.json | awk '/\"version\"/ { print $1 }'",
          "bluetoothd -v",
          "pulseaudio --version | cut -d= -f2| awk '{print $2}'",
          "uci get shifu.conf.LOG_LEVEL | cut -d= -f2 | awk '{print $1}'",
          "cat /sys/class/net/ath1/address",
          "cat /sys/class/net/ath0/address",
          "cat /sys/class/bluetooth/hci0/address",
          "iwpriv ath0 get_wps ",
          "iwpriv ath1 get_wps",
          "uci get shifu.conf.DEVICE_REGISTRATION_STATUS | cut -d= -f2 | awk '{print $1}'",
          "uci get shifu.conf.ALEXA_REGISTRATION_STATUS",
           "ls /dev | grep rtc0",
           "hwclock -u"]
cmd_list2=["ping 8.8.8.8 -c5",
          "fw_printenv DEVICE_ID",
          "uci show wireless.@wifi-iface[0].ssid",
          "hciconfig -a | awk '/^[a-z]/ {name=$NF}/Name:/{print $2}'",
          "cat /sys/class/net/eth0/operstate",
          "cat /sys/class/net/eth1/operstate",
          "ls /dev/ttyACM0",
          "ls /dev/ttyHS0",
          "gcontrol mcu get version",
          "uci get shifu.conf.TRONX_URL | cut -d= -f2 | awk '{print $1}'",
          "cxdish fw-version | cut -d: -f2 | awk '{print $1}'",
          "lsb_release -b",
           "cat /etc/shifu_version.json | awk '/\"version\"/ { print $1 }'",
          "bluetoothd -v",
          "pulseaudio --version | cut -d= -f2| awk '{print $2}'",
          "uci get shifu.conf.LOG_LEVEL | cut -d= -f2 | awk '{print $1}'",
          "cat /sys/class/net/ath1/address",
          "cat /sys/class/net/ath0/address",
          "cat /sys/class/bluetooth/hci0/address",
          "iwpriv ath0 get_wps ",
          "iwpriv ath1 get_wps",
          "uci get shifu.conf.DEVICE_REGISTRATION_STATUS | cut -d= -f2 | awk '{print $1}'",
          "uci get shifu.conf.ALEXA_REGISTRATION_STATUS",
           "ls /dev | grep rtc0",
           "hwclock -u"]


exp_output1=["0% packet loss",
            "[S0-9]{10}",
            "[a-z][A-Z][0-9]",
            "t.hub-BT-.{4}",
            "up",
            "up",
            "/dev/ttyACM0",
            "/dev/ttyHS0",
            "1",
            "https://tronxapi-staging.smartron.com/v2",
            "6",
            "TRQ",
             "[0-9]{1,}.[0-9]{1,}.[0-9]{1,}",
            "5.37",
            "8.0",
            "7",
            "[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}",
            "[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}",
            "[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}",
            "ath0      get_wps:1",
            "ath1      get_wps:1",
            "registered ",
            "registered",
             "rtc0",
             "seconds"]
exp_output2=["0% packet loss",
            "[S0-9]{10}",
            "t.hub-Wi-Fi-.{4}",
            "t.hub-BT-.{4}",
            "up",
            "up",
            "/dev/ttyACM0",
            "/dev/ttyHS0",
            "1",
            "https://tronxapi-staging.smartron.com/v2",
            "6",
            "TRQ",
            "[0-9]{1,}.[0-9]{1,}.[0-9]{1,}",
            "5.37",
            "8.0",
            "7",
            "[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}",
            "[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}",
            "[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}:[0-9,a-f]{2}",
            "ath0      get_wps:1",
            "ath1      get_wps:1",
            "not_registered ",
            "not-registered",
             "rtc0",
             "seconds"]
Testcase1="ps"
serv_cmd=[  "Mobile_Agent.py",
            "/usr/bin/sm_app",
            "Device_Manager_Gateway",
            "/usr/sbin/syslog-ng",
            "/usr/bin/SampleApp ",
            "/usr/bin/hw-control",
            "led-server",
            "Z3GatewayHost -n 1 -p /dev/ttyHS0",
            "zwave_app"]
Testcase2="ls /etc/init.d/"
daemon_cmd=[ "hyd",
            "lbd",
            "repacd",
            "ssid_steering",
            "wsplcd",
            "hyfi-bridging",
            "bluetoothd",
            "bluez-bccmd",
            "bluez-a2dp"]
PASS_count=0
FAIL_count=0
#status= False
out = subprocess.Popen("uci get shifu.conf.DEVICE_REGISTRATION_STATUS | cut -d= -f2 | awk '{print $1}'", stdout=subprocess.PIPE, shell=True)
Output = (out.communicate())
Output = str(Output)
Output1 = Output.replace('\n', " ")
#print Output
if "not_registered" in Output:
    status = False
    print status
else:
    status = True
    print status
with open('/root/FQC_LOG/FQC_Result.csv', 'w') as f:
    f.write("                                           \n")
    writer = csv.writer(f)
    writer.writerow(['SNo','Testcase Name','Testcase Output','Result'])

    ##  COMMANDS EXCEPT SERVICES & DAEMONS  ##
    if status == True:
         for i in range(len(cmd_list1)):
            if (i == 0):
                out = subprocess.Popen("ping 8.8.8.8 -c5", stdout=subprocess.PIPE, shell=True)
                SNo = str(i + 1)
                Testcase = str(testcase_list[i])
                Output = (out.communicate())
                Output = str(Output)
                Output1 = Output.replace('\n', " ")
                if (re.search(exp_output1[0], Output1, re.IGNORECASE)):
                    Result = 'PASS'
                    PASS_count = PASS_count + 1
                    print("pinged google")
                else:
                    out = subprocess.Popen("ping alibaba.com -c5", stdout=subprocess.PIPE, shell=True)
                    Output = (out.communicate())[0]
                    Output = str(Output)
                    Output1 = Output.replace('\n', " ")
                    if (re.search(exp_output1[0], Output1, re.IGNORECASE)):
                        Result = 'PASS'
                        PASS_count = PASS_count + 1
                        print("pinged alibaba")
                    else:
                        Result = 'FAIL'
                        FAIL_count = FAIL_count + 1
                        print("ping failed")
                # writer.writerow([SNo, Testcase, Output1, Result])
            else:
                out = subprocess.Popen([cmd_list1[i]], stdout=subprocess.PIPE, shell=True)  # stderr=subprocess.PIPE)
                SNo = str(i + 1)
                Testcase = str(testcase_list[i])
                command = str(cmd_list1[i])
                Output = (out.communicate())[0]
                Output = str(Output)
                Output1 = Output.replace('\n', " ")
                if (re.search(exp_output1[i], Output1, re.IGNORECASE)):
                    Result = 'PASS'
                    PASS_count = PASS_count + 1
                else:
                    Result = 'FAIL'
                    FAIL_count = FAIL_count + 1
            writer.writerow([SNo,Testcase,Output1, Result])
    else:
        for i in range(len(cmd_list2)):
            if (i == 0):
                out = subprocess.Popen("ping 8.8.8.8 -c5", stdout=subprocess.PIPE, shell=True)
                SNo = str(i + 1)
                Testcase = str(testcase_list[i])
                Output = (out.communicate())
                Output = str(Output)
                Output1 = Output.replace('\n', " ")
                if (re.search(exp_output2[0], Output1, re.IGNORECASE)):
                    Result = 'PASS'
                    PASS_count = PASS_count + 1
                    print("pinged google")
                else:
                    out = subprocess.Popen("ping alibaba.com -c5", stdout=subprocess.PIPE, shell=True)
                    Output = (out.communicate())[0]
                    Output = str(Output)
                    Output1 = Output.replace('\n', " ")
                    if (re.search(exp_output2[0], Output1, re.IGNORECASE)):
                        Result = 'PASS'
                        PASS_count = PASS_count + 1
                        print("pinged alibaba")
                    else:
                        Result = 'FAIL'
                        FAIL_count = FAIL_count + 1
                        print("ping failed")
                # writer.writerow([SNo, Testcase, Output1, Result])
            else:
                out = subprocess.Popen([cmd_list2[i]], stdout=subprocess.PIPE, shell=True)  # stderr=subprocess.PIPE)
                SNo = str(i + 1)
                Testcase = str(testcase_list[i])
                command = str(cmd_list2[i])
                Output = (out.communicate())[0]
                Output = str(Output)
                Output1 = Output.replace('\n', " ")
                if (re.search(exp_output2[i], Output1, re.IGNORECASE)):
                    Result = 'PASS'
                    PASS_count = PASS_count + 1
                else:
                    Result = 'FAIL'
                    FAIL_count = FAIL_count + 1
            writer.writerow([SNo, Testcase, Output1, Result])


    ##  FOR SERVICE COMMANDS  ##
    FAIL_count2 = 0
    out = subprocess.Popen([Testcase1], stdout=subprocess.PIPE, shell=True)  # type: Popen # stderr=subprocess.PIPE)
    SNo = str(int(SNo)+1)
    Output=(out.communicate())[0]
    Output = str(Output)
    Output1 = Output.replace('\n', " ")
    Result = ''
    failed_cmd=''
    for i in range(len(serv_cmd)):
        #SNo = str(i+int(S))
        #Testcase = str(testcase_list[i])
        if serv_cmd[i] not in Output1:
             Result = 'FAIL'
             FAIL_count2 = FAIL_count2 + 1
             #SNo = str(int(SNo) + 1)
             failed_cmd=failed_cmd + ' , '+serv_cmd[i]
             print serv_cmd[i]
    if FAIL_count2 !=0:
        writer.writerow([SNo,'Check Services', failed_cmd, Result])
    if (i==9 and Result != 'FAIL'):
        Result = 'PASS'
        PASS_count = PASS_count + 1
        #SNo = str(int(SNo) + 1)
        writer.writerow([SNo,'Check Services','All Services are ACTIVE', Result])

    ##  FOR DAEMON COMMANDS  ##
    FAIL_count3 = 0
    out = subprocess.Popen([Testcase2], stdout=subprocess.PIPE, shell=True)  # type: Popen # stderr=subprocess.PIPE)
    SNo = str(int(SNo) + 1)
    Output = (out.communicate())[0]
    Output = str(Output)
    Output1 = Output.replace('\n', " ")
    Result1 = ''
    failed_cmd1=''
    for i in range(len(daemon_cmd)):
        if daemon_cmd[i] not in Output1:
            Result1 = 'FAIL'
            FAIL_count3 = FAIL_count3 + 1
            #SNo = str(int(SNo) + 1)
            failed_cmd1 = failed_cmd1 + ' , ' + daemon_cmd[i]
            print daemon_cmd[i]
    if FAIL_count3 != 0:
        writer.writerow([SNo,'Daemons', failed_cmd1, Result1])
    if (i==8 and Result1 != 'FAIL'):
        Result1 = 'PASS'
        PASS_count = PASS_count + 1
        #SNo = str(int(SNo) + 1)
        writer.writerow([SNo,'Check Daemons','All Daemons are ACTIVE', Result1])
    f.close()
with open('/root/FQC_LOG/FQC_Result.csv', 'r+') as f1:
   f1.write('Total PASS'+','+str(PASS_count)+','+'Total FAIL'+','+str(FAIL_count+FAIL_count2+FAIL_count3))
   f1.close()
