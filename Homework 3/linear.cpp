// Return false if the somePredicate function returns false for at
// least one of the array elements; return true otherwise.
bool allTrue(const double a[], int n)
{
     if(n <= 0)
         return true;
     if(n == 1)
         return somePredicate(a[0]);
     else
         return somePredicate(a[n - 1]) && allTrue(a, n - 1);
}

// Return the number of elements in the array for which the
// somePredicate function returns false.
int countFalse(const double a[], int n)
{
    if(n <= 0)
        return 0;
    if(n == 1)
    {
        if (!somePredicate(a[0]))
            return 1;
        else
            return 0;
    }
    if(!somePredicate(a[n - 1]))
    {
        return 1 + countFalse(a, n - 1);
    }
    else
    {
        return countFalse(a, n - 1);
    }
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns false.  If there is no such
// element, return -1.
int firstFalse(const double a[], int n)
{
    if(n <= 0)
        return -1;
    int index = firstFalse(a, n - 1);
    if(!somePredicate(a[n - 1]))
    {
        if(index == -1)
            return n - 1;
    }
    return index;
}

// Return the subscript of the smallest double in the array (i.e.,
// the one whose value is <= the value of all elements).  If more
// than one element has the same smallest value, return the smallest
// subscript of such an element.  If the array has no elements to
// examine, return -1.
int indexOfMin(const double a[], int n)
{
    if(n <= 0)
        return -1;
    if(n == 1)
        return 0;
    int index = indexOfMin(a, n - 1);
    if(a[n - 2] < a[index])
    {
        return n - 2;
    }
    else
    {
        return index;
    }
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if(n2 == 0)
        return true;
    if(n1 == 0 && n2 == 0)
        return true;
    if(n1 == 0 && n2 > 0)
        return false;
    if(n1 == 1)
    {
        if(n2 > 1)
            return false;
        else if(n2 == 1)
        {
            if(a1[0] != a2[0])
                return false;
            else
                return true;
        }
    }
    if(a1[n1 - 1] != a2[n2 - 1])
    {
        return includes(a1, n1 - 1,a2, n2);
    }
    else
        return includes(a1, n1 - 1, a2, n2 - 1);
}
