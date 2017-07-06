import System.Collections.Generic;

namespace TestApp {
    internal var bool isValid;// { get;set;}

    private struct TestAppStruct {
        public var int A;
        public ptr var int B;
    }

    private enum TestAppEnum {
        MemberA = 0,
        MemberB = 5,
    }

    public func X(a int, b float) bool {
        return a >= b;
    }
}