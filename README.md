# fouc-langtool

Parser for localization files (.dat) found in FlatOut Ultimate Carnage, and potentially other BugBear Entertainment games.  
Files must be unpacked from the .bfs first (i used [bfstool](https://github.com/xNyaDev/bfstool)).  
Example usage:  
```
fouc-langtool.exe language0.dat languages.bed > out.txt
```  
Strings from the .dat will be combined with keys from the .bed and output to **out.txt** in the `KEY(ID)=<STRING>` format.
