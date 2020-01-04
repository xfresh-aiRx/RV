#!/usr/bin/perl

print qq?
<html>
<head>
<title>List of Doxygen warnings</title>
</head>
<body>
<h2>List of Doxygen warnings</h2>
There are two possible message types: "W" stands for "Warning",
"E" for error. Messages that are not related to a specific file
are included as plain text.
<table border=1>
<tr>
<th>File</th><th>Line</th><th>Type</th><th>Message</th>
</tr>
    ?;
    
@lines=<>;

for ($i=0; $i <=$#lines; $i++) {
  $_=$lines[$i];
  if (/^(.*):(\d+):\s*(\w+):\s*(.*)$/) {
    $file=$1;
    $line=$2;
    $type=$3;
    $msg=$4;
    $file =~ s/^.*src\///;
    $type =~ s/^([A-Za-z]).*$/\1/;
    if ($msg =~ /following/) {
      $msg.="<ul>\n";
      while ($lines[++$i] =~ /^\s/) {
        chop($lines[$i]);
        $msg.="<li>$lines[$i]</li>\n";
      }
      $i--;
      $msg.="</ul>";
    }
    print "<tr valign=top><td>$file</td><td>$line</td>\n<td>$type</td><td>$msg</td></tr>\n";
  } else {
    print "<tr><td colspan=4>$_</td></tr>\n";
  }
}
 
print qq?
</table>
</body>
</html>
    ?;

