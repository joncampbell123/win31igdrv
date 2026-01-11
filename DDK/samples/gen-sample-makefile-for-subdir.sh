#!/bin/bash
# Make building more convenient by generating a makefile to recursively make subdirs.
if [ -f "makefile" ]; then
    echo makefile already exists
    exit 1
fi

list=
for dir in *; do
    if [ -d "$dir" ]; then
        # i.e. the makefile is EATPAGES/EATPAGES
        if [[ -f "$dir/$dir" || -f "$dir/makefile" ]]; then
            list+="$dir "
        fi
    fi
done

if [ -z "$list" ]; then
    echo nothing to make
    exit 1
fi

# WARNING: output to makefile must use HARD TABS
cat >makefile <<_EOF
# auto-generated

all:
_EOF
for dir in $list; do
    if [ -f "$dir/makefile" ]; then
        cat >>makefile <<_EOF
	cd $dir
	\$(MAKE)
	cd ..
_EOF
    else
        cat >>makefile <<_EOF
	cd $dir
	\$(MAKE) -f $dir
	cd ..
_EOF
    fi
done
cat >>makefile <<_EOF

_EOF

