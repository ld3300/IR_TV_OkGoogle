/* LG TV Codes for model 50LB6000-UH
NEC IR protocol syntax is:
1st byte - device ID
2nd byte - device ID logical inverse
3rd byte - command
4th byte - command logical inverse
*/

#define TVIDCODE    0x20    // HEX ID for the TV

#define CHANNELUP   0x00    // Remote channel up button
#define KEYUP       0x02    // Remote up button
#define KEY0        0x08    // Remote key 0
#define AVMODE      0x0C    // Remote AV Mode button
#define PLAY        0x0D    // Remote Play button
#define POWERONOFF  0x10    // Remote Power button
// #define UNKNOWN     0x11    // Does something, not sure what
// #define MEDIARELATED 0x13   // Something to do with Media input, maybe switch to
#define BACK        0x14    // Remote Back button 
// #define MEDIARELATE 0x15    // Something to do with the media input
#define DIAGNOSIS   0x16    // Runs Self Diagnosis
#define KEY8        0x18    // remote 8 button
#define INPUTSAVAIL 0x19    // Rotates through available inputs
#define MAXVOL      0x21    // Turns volume all the way up, because I guess that is a thing
#define KEYOK       0x22    // Remote OK button
#define DIAGNOSIS2  0x25    // Also opens Self Diagnosis
#define KEY4        0x28    // Remote 4 button
// #define UNKNOWN     0x2A    // Does something, not sure what
#define CHANUP2     0x2C    // Does Channel up, but not same code as from remote
#define VOLUP2      0x2E    // Volume up, but not same code as from remote
#define LIST        0x32    // Remote List button
#define HDMI2       0x33    // Switch directly to HDMI2
#define DIAGNOSIS3  0x3D    // Also opens Self Diagnosis
// #define MEDIARELATE 0x3E    // Something to do with the media input
#define PCHECK      0x3F    // P-Check Factory Mode.  Looks like it is for checking video quality
#define VOLUMEUP    0x40    // Remote Volume up button
#define KEY2        0x48    // Remote 2 button
#define PICTURE2    0x49    // Picture settings but not same code as remote button
#define SOUND       0x4A    // Remote Sound button for Sound Mode
#define KEY1DOT     0x4E    // Remote 1 dot button
#define AUDIO       0x50    // Remote Audio button.  Doesn't seem to do anything
// #define MEDIARELATE 0x53    // Something to do with the media input
#define INFO        0x55    // Remote Info button
#define DIAGNOSIS4  0x56    // Also opens Self Diagnosis
#define FLASHBACK   0x58    // Remote Flashbk button - goes to previous channel
#define AVINPUT     0x5A    // Switch directly to AV Input
#define CRICKET     0x5C    // TV Cricket mode which swithes picture and sound settings to "Sport"
#define PAUSE       0x5D    // TV PAUSE button
// #define FACTORYDEF  0x5F    // Does a Factory Reset (might require pin 0413) Commented for safety
#define KEYRIGHT    0x60    // Remote right button
// #define UNKNOWN     0x61    // Does something, not sure what
#define HDMINEXT    0x63    // Cycle through HDMI inputs
#define KEY6        0x68    // remote 6 button
#define TVSWITCH    0x6B    // Toggles between tv modes (DTV, CABLEDTV)
#define RATIO43     0x6E    // Switch video ratio to 4:3
#define SLEEP       0x70    // Remote Sleep button, opens sleep menu
#define FASTFORWARD 0x71    // Remote Fast Forward button
#define HDMI1       0x73    // Switch directly to HDMI 1
#define FAMILY      0x74    // TV in Family mode, not sure what it does, maybe parental locks
// #define MEDIARELATE 0x77    // Something to do with the media input
#define FAVORITE    0x78    // Remote favorite button.  Cycles through favorite channels
// #define UNKNOWN     0x7A    // Does something, not sure what
#define SIMPLELINK  0x7E    // Opens Simplelink menu for controlling connected HDMI devices w remote
// #define POWERONLY   0x7F    // Puts in Power Only mode white screen with firmware version, commented for safety
#define CHANNELDOWN 0x80    // remote channel down button
#define KEYDOWN     0x82    // remote key down button
#define KEY4DOT     0x86    // remote 4 dot button
#define KEY1        0x88    // remote 1 button
#define STOP        0x8D    // remote stop button
#define KEY2DOT     0x8E    // remote 2 dot button
#define MUTE        0x90    // remote mute button
// #define UNKNOWN     0x96    // Does something, not sure what
#define KEY9        0x98    // remote 9 button
// #define UNKNOWN     0x99    // Does something, not sure what "This function not available now"
#define CAPTION     0x9C    // remote caption button brings up close caption menu
#define RATIO       0x9E    // remote ratio button brings up ratio menu and cycles
// #define UNKNOWN     0xA1    // Does something, not sure what
#define QMENU       0xA2    // Remote Q.Menu button 
#define POWEROFF    0xA3    // Turns TV OFF (not on)
#define KEY5        0xA8    // remote 5 button
#define ENERGYMODE  0xA9    // Cycles through energy saving modes
#define CHADDDEL    0xAA    // Delete/Add a channel to channel list
#define TVINFO      0xAB    // I think this just brings up the input info screen with clock???
#define CHDOWN2     0xAC    // Channel down but not same as remote
#define PICTURE     0xB2    // Remote picture button brings up picture mode
#define SCHECK      0xBF    // Factory Sound check menu
#define VOLUMEDOWN  0xC0    // remote volume down
#define SETTINGS    0xC2    // Remote settings button.  opens tv settings
#define INPUTINFO   0xC4    // brings up just info for current input (top left box)
#define KEY3DOT     0xC6    // Remote 3 dot button
#define KEY3        0xC8    // remote 3 button
#define CHANLIST    0xCD    // Brings up all channels list
#define VOLDOWN2    0xCE    // Volume down, just not same one as remote
#define INPUT       0xD0    // Remote input button - cycles through inputs
#define EXIT        0xDA    // remote exit button
#define OFFMODE     0xDD    // TV in off mode, think it turns off cricket and family mode
#define SERVICEMODE 0xDF    // Enters the secret service mode
#define KEYLEFT     0xE0    // remote left button
// #define UNKNOWN     0xE1    // Does something, not sure what
#define KEY7        0xE8    // remote 7 button
#define TVINFO2     0xEB    // Also brings up TV info top box
#define RATIO169    0xEE    // Sets TV Ratio to 16:9
#define TVBUTTON    0xF0    // remote TV button.  Switches to TV Input and cycles cable and tv
#define REWIND      0xF1    // Remote rewind button
#define RATIOZOOM   0xF5    // Sets TV Ratio to Zoom
#define COMPONENT   0xFD    // Sets TV to Component Input
#define EZADJUST    0xFF    // EZ Adjust option for secret menu