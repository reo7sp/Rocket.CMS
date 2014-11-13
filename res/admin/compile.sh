#!/bin/bash

echo "Cleaning up"
find -name "*.html" -print0 | xargs -0 rm

echo "Generating /"
tpage pages-list.tt2 > pages-list.html
tpage pages-edit.tt2 > pages-edit.html
tpage template-list.tt2 > template-list.html
tpage template-edit.tt2 > template-edit.html
tpage files-list.tt2 > files-list.html
tpage files-edit.tt2 > files-edit.html

echo "Generating /css"
lessc css/style.less > css/style.css

echo "Formatting"
find \( -name "*.html" -o -name "*.css" \) -print0 | xargs -0 perl -i -pe 's/\n/ /g'
find \( -name "*.html" -o -name "*.css" \) -print0 | xargs -0 perl -i -pe 's/\s+/ /g'

echo "Done"
