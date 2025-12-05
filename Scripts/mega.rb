#!/usr/bin/env ruby

require "fileutils"

if ARGV.size != 3
	puts "   ruby mega.rb ../Source ../Apps ../Target"
	return
end

source=ARGV[0]
apps=ARGV[1]
target=ARGV[2]

FileUtils.rm_rf(target)
unless Dir.exist?(target)
  Dir.mkdir(target)
end

Dir.glob("#{source}/*.cpp").each do |file|
  file_base_name = File.basename(file, ".cpp")
  FileUtils.cp(file, File.join(target, "#{file_base_name}.ino"))
end

Dir.glob("#{source}/*.h").each do |file|
  FileUtils.cp(file, target)
end

Dir.glob("#{apps}/*.cpp").each do |file|
  file_base_name = File.basename(file, ".cpp")
  FileUtils.cp(file, File.join(target, "#{file_base_name}.ino"))
end

["Arduino.ino", "Arduino.h", "main.ino", "Wire.ino", "Wire.h","App.ino"].each do |f|
  FileUtils.rm(File.join(target,f))
end

contents = File.read(File.join(source,"App.cpp"))
File.open(File.join(target,"Mega.ino"), "w") do |file|
  file.write("#define MEGA 1\n")
  file.write(contents)
end

puts "PORT=`ls /dev/cu.usb*`"
puts "arduino-cli compile --build-property build.extra_flags=-DMEGA --fqbn arduino:avr:mega --build-path=Mega/sketch Mega"
puts "arduino-cli upload -p $PORT --fqbn arduino:avr:mega --input-dir Mega/sketch Mega"