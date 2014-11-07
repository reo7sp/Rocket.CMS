#!/bin/bash

echo "Cleaning up"
find -name "*.html" -print0 | xargs -0 rm

echo "Generating /"
tpage pages-list.tt > pages-list.html
tpage pages-edit.tt > pages-edit.html

echo "Generating /css"
lessc css/style.less > css/style.css

echo "Formatting"
find \( -name "*.html" -o -name "*.css" \) -print0 | xargs -0 perl -i -pe 's/\n/ /g'
find \( -name "*.html" -o -name "*.css" \) -print0 | xargs -0 perl -i -pe 's/\s+/ /g'

count=$(find -name "*.html" | wc -l)
echo "Generated $count pages"
