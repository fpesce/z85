require "test/unit"
require "z85"

class TestZ85 < Test::Unit::TestCase
    def test_hello_world
        string_test = "\x86\x4f\xd2\x6f\xb5\x59\xf7\x5b"
        test_encode = Z85::encode(string_test)
        assert_equal 'HelloWorld', test_encode
        test_decode = Z85::decode(test_encode)
        assert_equal string_test, test_decode
    end
end
