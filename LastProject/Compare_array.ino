int Compare(int a[], int b[]) {//배열 비교기
  int check = 1;
  for (int count = 0; count <= 3; count += 1) {
    if (a[count] != b[count]) {
      check += 1;
      break;
    }
  }
  if (check == 1) {
    return 1;
  }
  else {
    return 0;
  }
}
