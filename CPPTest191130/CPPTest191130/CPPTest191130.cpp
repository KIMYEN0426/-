// CPPTest191130.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;

void func(shared_ptr<int> _P) {

}

int main()
{
	shared_ptr<int> a(new int(10));
	*a = 20;
	func(a);

    return 0;
}

