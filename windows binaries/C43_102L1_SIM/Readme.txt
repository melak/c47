20 Jan 2021 release pre-alpha REL102.

This is the pre-alpha version of the C43 modifications made on top of the WP43S prelim alpha emulator. Do not expect it to work. It probably does not.

Current releases, pre-programming:
Rel49 : Upcoming final release as soon as the shift lock keys on L1 are settled.

Current release, programming model, based on WP43S in progress:
Rel101POC: 2021-01-10. 


The basic C43 write-up is on the first page of https://forum.swissmicros.com/viewtopic.php?f=2&t=2216. The source code is on https://gitlab.com/Jaymos/wp43c.

If you do test, please submit well structured comments to the above group. With well structured, I mean documented repeatable errant behaviour, preferably listed in a group of errors, and/or comments, all in one (or few) post(s), numbered for reference in follow up posts.

Questions welcomed on the above group.

Praise and compliments to Martin, Pauli and Walter for the work on the base project WP43S. The C43 work done thus far is way less than the effort made by these guys, and the C43 cannot exist without the base code of the WP43S.

---
Notes:

1. Keyboard re-arrangement for ALPHA maode.

####################################################################################
2. Be sure to delete the previous backup.bin file prior to using this version. Also, on the DM42 hardware, do a CLR RESET to re-initialize the calculator.
####################################################################################

3. To make a screen shot from the DM42 hardware screen, keep in shift f/g and press DISP. The screen shot will appear in the DM42 flash disk, under /SCREENS/

4. Try SHOW. Up and Dn. With and without menus visible. If there are multiple screens indicated by the ^v arrow on screen, arrows operate the menu. If no or one screen only, then the arrows control the SHOW command and shows all named registers.

5. Try XEQM on long press XEQ, then XEQM. These are softmenu buttons reflecting 18 user RPN program files on the flash disk under /PROGRAMS/. 

6. Try some notable differences form the 43S, i.e.

11a. BASE menu, 
11b. Alpha mode multi line text editor,
11c. Longpress EXIT clears input buffers
11d. Longpress backspace clear alpha buffer
11e. STAT / PLOT. DEMO data is availble. Scroll down using Dn.
11f. Alpha entry.

7. Specifically, the numlock and capslock shifts have been modified.

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
