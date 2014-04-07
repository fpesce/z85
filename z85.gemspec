Gem::Specification.new do |s|
    s.name          = 'z85'
    s.version       = '0.1.0'
    s.date          = '2014-04-07'
    s.summary       = "ZeroMQ Base-85 Encoding Algorithm"
    s.description   = "This implements Z85, a format for representing binary data as printable text. Z85 is a derivative of existing Ascii85 encoding mechanisms, modified for better usability, particularly for use in source code."
    s.authors       = ["Francois Pesce"]
    s.email         = 'francois.pesce@gmail.com'
    s.require_paths = ["lib"]
    s.extensions    = ["ext/z85/extconf.rb"]
    s.files         = ["lib/z85.rb", "ext/z85/z85.c", "Manifest.txt", "History.txt", "README.txt", "Rakefile", ".autotest", ".gemtest", "bin/z85"]
    s.test_files    = ["test/test_z85.rb"]
    s.add_dependency "rake-compiler",        "0.7.9"
    s.homepage      =
        'http://github.com/fpesce/z85'
    s.license       = 'MIT'
end
