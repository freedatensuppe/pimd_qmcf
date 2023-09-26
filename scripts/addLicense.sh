for i in $(find src/ tests/ include/ -regex ".*\.\(hpp\|cpp\|c\|h\)$"); do

    if ! grep -q Copyright $i; then

        echo "Adding license to $i"
        cat config/licenseHeader.txt $i >$i.new && mv $i.new $i

    fi

done
