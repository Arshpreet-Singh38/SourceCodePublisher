Source Code Publisher
==============================================================
Overview:

- This application takes a path, patterns (*.h, *.cpp) and regex ([A-D](.*)) as input and 
  recursively searches all the directories present in the path provided. It finds all the 
  files that match the given patterns.
  
- Thereafter, these files are filtered out to obtain the files matching the regular
  expressions.
  
- These files are then converted into html files, with all its contents intact. To perform 
  this conversion efficiently and correctly, the escape sequences are replaced by their
  corresponding strings (for eg. '<' with "&lt", '>' with "&gt", '&' with "&amp").
  
- After performing the conversion, the files are displayed in the browser sequentially i.e. 
  it waits for the window to be killed to pop-up the next file in the browser which
  is done through processes.
  
===============================================================

*The application is developed in Microsoft Visual Studio 2019 C++ *
  
===============================================================
  
  How to run the application?
  
  - compile.bat : compiles all the packages.
  - run.bat : Used to run the application.
  
 ===============================================================
