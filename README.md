# FileToCodeConverter
File To Code Converter

<h2>Description</h2>

The FileToCodeConverter is a Program which converts any file to a C/C++ code (header or a cpp file)! This is useful when you need to put those files inside your executable in a cross-platform way. For example, when you need to put an image to an executable to later use it on your app.

<h2>Features</h2>

<ul>
<li><b>2 types of conversion:</b></li>
  <ul>
  <li>Procedural converter (Der Alte)(oldmode) - Fully developed 'n' working!</li>
  <li>Object-Oriented converter (newmode) - Still InDev.</li>
  </ul>
<li><br><b>2 modes of putting file data into an array:</b></li>
  <ul>
  <li>Unsigned char array with bytes represented as Numbers (inefficient, takes much space)</li>
  <li>Char array with bytes represented as ASCII Characters (very efficient, takes only little bit more space than the original) - Even special characters are interpreted correctly (using escape sequences)</li>
  </ul>
<li><br><b>Several ConvertModes (different file interpretation and different metadata)</b></li>
  <ul>
  <li>Binary Mode (default) - only file data in a buffer. Metadata - file size.</li>
  <li>Text Mode - content written as a C-String of chars.</li>
  <li>Picture Mode (works with BMP Files) - pixel RGB(A) values written in a buffer. Metadata - picture width, height, bits per pixel, file size.</li>
  <li>Auto mode - chooses mode automatically based on file extension.</li>
  <li>More to come!</li>
  </ul>
<li><br><b>UI features - on Windows, you select input and output files in an Explorer window, on other OS's - on a CMD interface.</b></li>
</ul>

<h2>TODOS</h2>
<ul>
<li>More efficient file reading, chunk based.</li>
<li>More modes, more metadata options. (not really needed)</li>
<li>Possible merging with another project, "Kawaii File Manipulator", with FileToCode added as an extension.</li>
</ul>
