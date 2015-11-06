# etst
Elastic Tabstop expander for the CLI

etst is  a command line program to layout text using elastic tabstops as 
[described by Nick Gravgaard](http://nickgravgaard.com/elastic-tabstops/).
It is written as a result of a [suggestion by waterlink](https://github.com/manastech/crystal/issues/1682#issuecomment-145386137).
etst reads text to layout from stdin, and writes to stdout.

# Usage
`etst` [indent width] < file
