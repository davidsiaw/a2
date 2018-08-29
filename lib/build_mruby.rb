# This file is used by Visual Studio to build mruby.
# Other non-windows platforms can build mruby is a much more elegant manner.

config_string = ARGV[0]
platform_string = ARGV[1]
ENV["MRUBY_BUILD_DIR"] = "#{config_string}_#{platform_string}_build"
exec "ruby minirake"
