# -*- coding: utf-8 -*-
=begin
[usage]
 
1. download this file to your desktop
2. open your Terminal.app
 
$ sudo gem install oauth
$ sudo gem install json
$ cd ~/Desktop
$ ruby hackpad.rb
=end
 
require 'rubygems'
require 'json'
require 'oauth'
require 'net/http'
 
 
# hackpad api
# https://hackpad.com/Hackpad-API-v1.0-k9bpcEeOo2Q#:h=REST-API
url = "https://hackpad.com"
key = "CKj4vKwqaYr"
secret = "q6eEiPxV2smpJrsDCS4H327rdhp39oTZ"
dir_name = "my_hackpad"
 
# OAuth 1.0a 0-legged!!
consumer = OAuth::Consumer.new(
                               key,
                               secret,
                               :site => url,
                               :request_token_path => "",
                               :authorize_path => "",
                               :access_token_path => "",
                               :http_method => :get)
 
access_token = OAuth::AccessToken.new consumer
 
 
# ･゜･*:.｡..:*･'｡. .｡.:*･゜･*
# get the padid list of all pages
res = access_token.get("/api/1.0/pads/all")
pad_ids = JSON.parser.new(res.body).parse()
 
 
# ･゜･*:.｡..:*･'｡. .｡.:*･゜･*
# get the pad content + saving to file
 pad_ids.each{|pad_id|
  
  file_format = "md" # ("txt", "html", or "md")
   endpoint = "/api/1.0/pad/#{pad_id}/content.#{file_format}"
  
  res = access_token.get(endpoint).body
  pad_content = res.split("\n");
  
  first_line = pad_content.delete_at(0)
  title = (first_line == nil) ? pad_id : first_line
  body = pad_content.join("\n")
  
  title.sub!(/#\s+/, "") # remove "# "
  title.gsub!(/\//, "_") # remove "/"
  
   file_name = "#{dir_name}/#{title}.#{file_format}"
  
  Dir::mkdir(dir_name) unless FileTest::directory?(dir_name)
  
   p "start saving: #{file_name}"
  
  f = File::open(file_name, 'w')
  f.puts body
  f.close
  
  p "saved."
  p "------"
 }
