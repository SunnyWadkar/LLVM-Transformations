int main ()
{
  int result;
  int n = 1;
  int j = 0;
  while (n++ < 100) {
	  result = 0; // This code needs to be hoisted
	  result += 2; // This code needs to be hoisted
	  result += 3; // This code needs to be hoisted
	  result *= 4; // This code needs to be hoisted
	  result /= 2; // This code needs to be hoisted
    j = j + n;
  }

  return result;
}