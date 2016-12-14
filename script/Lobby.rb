#!/usr/bin/env ruby

this_dir = File.expand_path(File.dirname(__FILE__))
lib_dir = File.join(this_dir, '../build')
$LOAD_PATH.unshift(lib_dir) unless $LOAD_PATH.include?(lib_dir)

require 'grpc'
require 'server_services_pb.rb'

def main
  stub = Comm::Server::Stub.new('localhost:6666', :this_channel_is_insecure)
  stub.add_user(Comm::User.new(m_Name:"William"))
    
end

main
