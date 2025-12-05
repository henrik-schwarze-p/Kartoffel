#!/usr/bin/env ruby
#~/Kartoffel/appStore/bin $ ./g_icon.rb ../public/generated/408/Icons ../../resources/Icons ../public/generated/408/Source
require "fileutils"
require "securerandom"
require "open3"

def write_h(br, filename, what)
  File.open(br + "/" + filename + ".inc.h", "w") { |f| f.write(what) }
end

def create_icon_representation(file, destination)
  script_dir = ENV["AOSDIR"] + "/scripts"
  uid = SecureRandom.uuid
  dst = "/tmp/txt.#{uid}"
  Open3.capture3("convert #{file} -colorspace RGB txt:-  | sed -e '/Magi/d' -e 's/:.*#/,/' -e 's/ .*//' | tr ',' '\\n' > #{dst}")
  stdout, _, _ = Open3.capture3("./IC #{dst} icon #{destination}")
  File.delete(dst)
  stdout
end

icons_dir = ARGV[0]
resource_icons_dir = ARGV[1]
dest_dir = ARGV[2]

files = Dir.glob("#{icons_dir}/*.png") + Dir.glob("#{resource_icons_dir}/*.png")

icons_h = ""
files.each_with_index { |file, id| icons_h += create_icon_representation(file, id) + "\n" }
iconlib = "const int* iconData[] = {#{(0..files.size - 1).map { |f| "_Icon#{f}" }.join(",")}};"

write_h(dest_dir, "Icons", icons_h)
write_h(dest_dir, "Icons4", iconlib)