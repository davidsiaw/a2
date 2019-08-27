MRuby::Build.new do |conf|
  # load specific toolchain settings

  # Gets set by the VS command prompts.
  if ENV['VisualStudioVersion'] || ENV['VSINSTALLDIR']
    toolchain :visualcpp
  else
    toolchain :gcc
  end

  conf.gembox 'default'

  # This needs to be right at the bottom
  # otherwise it will interfere with other gems building
  conf.gem github: 'mattn/mruby-require'
end
