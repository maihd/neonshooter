./premake5 gmake

cd Build/gmake

# Select build config: debug_x32, debug_x64, release_x32, release_x64
make config=release_x64 CC=gcc

rm -rf ../../NeonShooter

mv bin/x64/Release/NeonShooter ../../NeonShooter

cd ../../

./NeonShooter

rm -rf NeonShooter