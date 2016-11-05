#!/usr/bin/env ruby

this_dir = File.expand_path(File.dirname(__FILE__))
lib_dir = File.join(this_dir, '../build')
$LOAD_PATH.unshift(lib_dir) unless $LOAD_PATH.include?(lib_dir)

require 'grpc'
require 'extcomm_services_pb.rb'

def main
  stub = Comm::ExternalCommunication::Stub.new('localhost:50051', :this_channel_is_insecure)
  stub.connect_to_server(Comm::EndPoint.new(m_Ip: "#{ARGV[0]}", m_Port: 1213))
end

main
