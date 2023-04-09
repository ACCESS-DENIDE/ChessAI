#pragma once


static int DivDwnFlr(int num, int div) {
	if (div > num) {

		return 0;

	}
	else {

		return (num - (num % 8)) / div;

	}


}