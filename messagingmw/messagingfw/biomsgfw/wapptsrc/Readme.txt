The test harness for the WAP Access Point Parser requires binary data files in the directory <\epoc32\WINS\c:\Biotest\twap>.

These files are WBXML WAP settings messages. They should have name of the form *.wmlc and have been created by the WBXML encoder at the web-site <http://wfldemo.wirelessfuture.com/wap/wapdemos.asp> from XML WAP settings messages uploaded to the site.

The test harness simply creates new messages in the inbox with the binary data in the message body and calls the WAP Access Point Settings Parser to parse them.