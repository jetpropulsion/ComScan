# ComScan
Crawls existing Microsoft header files and (re)generates found GUID, CLSID and similar UUID constants for reuse.

Important note
--------------
Source code is written using one tab for two spaces.
I'm sorry because it looks terrible on 1:8, but that's my preference.

Ingredients and tools you will need to build this project
---------------------------------------------------------
- ActiveState perl, this project is developed using "perl 5, version 20, subversion 2 (v5.20.2) built for MSWin32-x64-multi-thread"
- Microsoft Visual Studio 2015 Community Edition
- Source code (provided here)

Introductory Excuse & Note
--------------------------
My dear fellow Perl programmers, I'm making excuse in advance, but I'll be a little bit more detailed about configuring the script,
because there are many excellent C++ programmers out there who choke when they hear single "p" out of "perl".


Build order
-----------

- Open and configure ComScan.pl perl script in your favorite text editor, and change the following two variables:

  #########
  ### 1 ###
  #########

  <pre>my $file_regex = qr/^[^\.]+\.(?:h|hpp|hh|inl|c|cpp|cc|cxx)$/io;</pre>
  
  Basically, even if you don't know Regular Expressions (regex), you can configure which files will be scanned during
  recursive directory analysis following way - just change visible file extensions in this particular regex to something that
  better matches your environment. In this regex following extensions should be obvious:
  "h", "hpp", "hh", "inl", "c", "cpp", "cc" and "cxx".

  In order to examine *.txt and *.bat files, in addition to existing, one would write:

  <pre>my $file_regex = qr/^[^\.]+\.(?:h|hpp|hh|inl|c|cpp|cc|cxx|txt|bat)$/io;</pre>

  #########
  ### 2 ###
  #########

  <pre>
  @include_dirs - this is array, currently initialized to my environment as follows:
  
  my @include_dirs =
  (
	  '%ProgramFiles(x86)%\Microsoft SDKs\Windows\v7.1A',       ### Search inside official Windows SDK
	  '%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\VC',    ### Search inside Visual C++ standard libraries
	  '%USERPROFILE%\Documents\Visual Studio 2015\Projects'     ### <== I just added Projects in My Documents default location as an example
  );
  </pre>

  as you can notice, paths can be provided as unexpanded strings.
  
  #########
  ### 3 ###
  #########

  Build can now start, first, generate C++ header file the following way:
  
  - From the command line, change current directory to the directory you downloaded/unzipped the solution
  
  - Execute modified ComScan.pl redirecting its standard output to the "ScannedData.h" file, like this:
    <pre>
      C:\Perl64\bin\perl.exe ComScan.pl >"ScannedData.h"
    </pre>
      (you will still see the progress and analyzed file in real time, because script writes such messages to stderr)
  
  - Open solution file from Microsoft(R) Visual Studio(TM) (C)2015 and build it
  
  - Project executable (ComScan.exe) is capable of finding arbitrary UUID in its (re)built database, like this example:
      <pre>
      C:\GroundZero\ComScan\> ComScan.exe "00000100,0000,0000,c0,00,00,00,00,00,00,46"

      GUID parsed, searching... GUID is found:

      GUID { 0x00000100, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } }         //CRC32=0xb059332f
      0: header="C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Include\ObjIdl.h", macro="MIDL_INTERFACE", name="IEnumUnknown"
      </pre>

  #########
  ### 4 ###
  #########

  Good luck fellow coders!
  
