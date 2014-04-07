# -*- ruby -*-

require "rubygems"
require "hoe"

# Hoe.plugin :compiler
# Hoe.plugin :gem_prelude_sucks
# Hoe.plugin :inline
# Hoe.plugin :racc
# Hoe.plugin :rcov

HOE = Hoe.spec "z85" do
    developer("Francois Pesce", "francois.pesce@gmail.com")
#    self.readme_file   = 'README.rdoc'
#    self.history_file  = 'CHANGELOG.rdoc'
    self.extra_rdoc_files  = FileList['*.rdoc']
    self.extra_dev_deps << ['rake-compiler', '>= 0']
    self.spec_extras = { :extensions => ["ext/z85/extconf.rb"] }
    license "MIT"
end

require "rake/extensiontask"

Rake::ExtensionTask.new(HOE.name, HOE.spec) do |ext|
    ext.lib_dir = File.join('lib', 'z85')
end

Rake::Task[:test].prerequisites << :compile

# vim: syntax=ruby
