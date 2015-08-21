cmake CMakeLists.txt
make install
if [ ! -d ./bin/bundle ]; then
    mkdir bin/bundle
	mkdir bin/bundle/my_bundle
	mkdir bin/bundle/caller_bundle
fi

mv ./bundle-sample/libmy_bundle.so ./bin/bundle/my_bundle/my_bundle.so -f
cp ./bundle-sample/my_bundle/*.inf ./bin/bundle/my_bundle -f

cp ./bundle-sample/libcaller_bundle.so ./bin/bundle/caller_bundle/caller_bundle.so -f
cp ./bundle-sample/caller_bundle/*.inf ./bin/bundle/caller_bundle/ -f

