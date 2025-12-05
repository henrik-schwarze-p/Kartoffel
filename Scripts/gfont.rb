#!/usr/bin/env ruby
require "fileutils"
require 'open3'
require 'securerandom'

destfile = ARGV[0]
fontfile = ARGV[1]
selector = ARGV[2]

bin_dir = ENV["AOSDIR"] + "/appStore/bin"

uid = SecureRandom.uuid
dst = "/tmp/#{uid}"
Open3.capture3("convert #{fontfile} -background white -flatten bmp3:#{dst}")
stdout, _, _ = Open3.capture3("./IC #{dst} font #{selector}")
File.delete(dst)
stdout

File.open(destfile, "w") { |f| f.write(stdout) }

