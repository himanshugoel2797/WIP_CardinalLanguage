import System.Collections.Generic;

namespace TestApp {
    public class TestAppClass {
        internal bool isValid { get;set;}

        public TestAppClass(){
            isValid = true;
        }

        public bool IsValid {
            return isValid;
        }
    }

    private struct TestAppStruct {

    }

    private enum TestAppEnum {
        MemberA = 0,
        MemberB = 5,
    }
}