mp3tags
by Arvid Norberg <arvid@iname.com>

This is a Tracker add-on which edits mp3 tags (ID3-tags). You can use it on single files, but the real functionality you'll find if you run it on multiple files.

Pattern matching
There's a pattern matching function that reads the filename and sets tag data depending on it. If, for example, all you mp3 files are stored in the following format: "artist - title.mp3" you can set the artist and title field, on all files, to these names, simply by check the box "use pattern matching" and use the default pattern. I think you'll get the idea if you just try different patterns.

If any of the selected files have the wrong MIME-Type (i.e. not application/x-mpeg) or it doesn't match the pattern or some other error occurs, an alert will tell you that.

NOTE: You need the liblayout.so (by Marco Nelissen) to run the add-on.

send bugreports/feedback to arvid@iname.com
