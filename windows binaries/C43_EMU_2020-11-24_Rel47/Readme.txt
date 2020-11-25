24 Nov 2020 release pre-alpha REL47



This is the pre-alpha version of the C43 modifications made on top of the WP43S prelim alpha emulator. Do not expect it to work. It probably does not.

The basic C43 write-up is on the first page of https://forum.swissmicros.com/viewtopic.php?f=2&t=2216. The source code is on https://gitlab.com/Jaymos/wp43c.

If you do test, please submit well structured comments to the above group. With well structured, I mean documented repeatable errant behaviour, preferably listed in a group of errors, and/or comments, all in one (or few) post(s), numbered for reference in follow up posts.

Questions welcomed on the above group.

Praise and compliments to Martin, Pauli and Walter for the work on the base project WP43S. The C43 work done thus far is way less than the effort made by these guys, and the C43 cannot exist without the base code of the WP43S.

---
Notes:

1. Note the dots above the shift key. One tap is for yellow shifted labels, a.k.a. f-shift. Two taps for blue shifted labels, a.k.a. g-shift. Three taps clears the shift and pops up the HOME menu.

2. Triple shift mode (HOME.3) can be disabled using the switch in [MODE], [Dn]. Triple shift is also influenced by the triple shift timing switch SH.3T. Shift time out is controlled by SH_4s. None of these settings are really end user settings, and they are work in progress.

3. The setting for triple shift HOME (HOME.3) is enabled by default. Triple shift accesses HOME. Or [g][7] does. Triple shift or EXIT to restores to previous menu.

4. HOME (triple shift) comprises a number of home screens and a number of keyboard mirroring menus. Arrow [Dn] to access the next screen in a circular fashion.

5. The latest CSS and PNG file are supplied. These are different from the main project, WP43S.

6. A lot of effort was put into the emulator keyboard control to make it a useful Windows Calc replacement. Welcome to test that. Clipboard control was added, via the usual CTRL-C, and also -H, -D, -A, -X.


####################################################################################
7. Be sure to delete the previous backup.bin file prior to using this version. Also, on the DM42 hardware, do a CLR RESET to re-initialize the calculator.
####################################################################################


8. To make a screen shot from the DM42 hardware screen, keep in shift f/g and press DISP. The screen shot will appear in the DM42 flash disk, under /SCREENS/

9. Try SHOW. Up and Dn. With and without menus visible. VIEW temporarily (until 43S develops the VIEW command) has the original WP43S SHOW function for comparison.

10. Try KEYS on the simulator. This menu has various key layouts, including a WP43S layout if you have re-painted your DM42 keys that way.

11. Try XEQM on long press XEQ, then XEQM. These are softmenu buttons reflecting 18 user RPN program files on the flash disk under /PROGRAMS/. 

12. Try the new BASE menu. And Up and Dn to further screens.

13. Try the new Alpha mode multi line text editor.

14. Try the new longpress EXIT clear input buffer.

15. Try the new STAT / PLOT

16. Try the new FIFO on all softmenus

17. Try the STAST/PLOT menu system. DEMO data is availble. Scroll down using Dn.

18. Try the key label changes!

---

Jaymos
Jaco Mostert


----------------------ORIGINAL WP43S README COPIED FROM WP43S GITLAB PROJECT---------------------------------------------
Windows 64 bit binaries


This binary is provided for your convenience.


You must install the latest GTK3+ libraries. 

If you do not already have them: download and install the latest release of the gtk3-runtime (not gtk2-runtime) for win64 here https://github.com/tschoonj/GTK-for-Windows-Runtime-Environment-Installer/releases
Install
or re-install the WP43S_StandardFont.ttf font. The WP43S_NumericFont.ttf font isn't needed.


Copy wp43s.exe in a new or pre-existing directory.

Copy dm42l.png, dm42lshort.png and wp43s_pre.css from the root of this gitlab project to the same directory
.
If there is a backup.bin file in your already existing directory: delete it
.

Run wp43s.exe and enjoy.
