#include <iostream>
#include <vector>
using namespace std;

void bubbleSort1(vector<int>& arr) {
    for( int i = 0; i < arr.size(); ++i ) {
        for( int j = 0; j < arr.size() - 1 - i; ++j ) {
            if( arr[j] > arr[j+1] )
                swap(arr[j], arr[j+1]);
        }
    }
}

void bubbleSort2(vector<int>& arr) {
    bool swapped = true;  //true表示发生过交换，还需要再进行比较
    for( int i = 0; i < arr.size(); ++i ) {
        if( swapped == false ) break;
        swapped = false;  //设为false，如果发生交换，则将其置为 true
        for( int j = 0; j < arr.size() - 1 - i; ++j ) {
            if( arr[j] > arr[j+1] ) {
                swap(arr[j], arr[j+1]);
                swapped = true;    // 表示发生了交换    
            }
        }
    }
}

void bubbleSort3(vector<int>& arr) {
    bool swapped = true;  //true表示发生过交换，还需要再进行比较
    int indexOfLastUnsortedElement = arr.size() - 1;
    int swappedIndex = -1;
    while( swapped ) {
        swapped = false;
        for( int i = 0; i < indexOfLastUnsortedElement; ++i ) {
            if( arr[i] > arr[i+1] ) {
                swap(arr[i],arr[i+1]);
                swapped = true;
                swappedIndex = i;
            }
        }
        indexOfLastUnsortedElement = swappedIndex;
    }
}

void selectionSort1(vector<int> &nums) {
    int minIndex;
    for( int i = 0; i < nums.size() - 1; ++i ) {
        minIndex = i;
        for( int j = i + 1;j < nums.size(); ++j ) {
            if( nums[minIndex] > nums[j] )
                minIndex = j;
        }
        swap( nums[minIndex], nums[i] );
    }
}

void selectionSort2(vector<int> &nums) {
    int minIndex,maxIndex;
    //i只需要遍历一半
    for( int i = 0; i < nums.size() / 2; ++i ) {
        minIndex = i;
        maxIndex = i;
        for( int j = i + 1;j < nums.size() - i; ++j ) {
            if( nums[minIndex] > nums[j] )
                minIndex = j;
            if( nums[maxIndex] < nums[j] )
                maxIndex = j;
        }
        // 如果 minIndex 和 maxIndex 都相等，那么他们必定都等于 i，且后面的所有数字都与 arr[i] 相等，此时已经排序完成
        if( minIndex == maxIndex ) break;
        swap( nums[minIndex], nums[i] );

        // 如果最大值的下标刚好是 i，由于 arr[i] 和 arr[minIndex] 已经交换了，所以这里要更新 maxIndex 的值。
        //如果最大值的下标等于 i，也就是说 arr[i] 就是最大值，由于 arr[i] 是当前遍历轮次的首位，它已经和 arr[minIndex] 交换了，所以最大值的下标需要跟踪到 arr[i] 最新的下标 minIndex。
        if( maxIndex == i ) maxIndex = minIndex;
        swap( nums[maxIndex], nums[nums.size() - 1 - i] );
    }
}

void insertSort1(vector<int> &nums) {
    // 从第二个数开始，往前插入数字
    for( int i = 1 ; i < nums.size(); ++i ) {
        int curNum = nums[i];
        int j = i - 1;
        // 寻找插入位置的过程中，不断地将比 currentNumber 大的数字向后挪
        while( j >= 0 && curNum < nums[j] ) {
            nums[j + 1] = nums[j];
            j--;
        }
        // 两种情况会跳出循环：
        // 1. 遇到一个小于或等于 currentNumber 的数字，跳出循环，currentNumber 就坐到它后面。
        // 2. 已经走到数列头部，仍然没有遇到小于或等于 currentNumber 的数字，也会跳出循环，此时 j 等于 -1，currentNumber 就坐到数列头部。

        nums[j+1] = curNum;
    }
}

void insertSort2(vector<int> &nums) {
    // 从第二个数开始，往前插入数字
    for( int i = 1 ; i < nums.size(); ++i ) {
        int j = i;
        // 寻找插入位置的过程中，不断地将比 currentNumber 大的数字向后挪
        while( j >= 1 && nums[j] < nums[j - 1] ) {
            swap( nums[j], nums[j-1] );
            j--;
        }
    }
}

void shellSort1(vector<int>& nums) {
    //// 间隔序列，在希尔排序中我们称之为增量序列
    for( int gap = nums.size() / 2; gap > 0; gap /= 2 ) {
        // 分组
        for( int groupStartIndex = 0; groupStartIndex < gap; ++groupStartIndex ) {
            for( int currentIndex = groupStartIndex + gap; currentIndex < nums.size(); currentIndex += gap ) {
                // currentNumber 站起来，开始找位置
                int currentNumber = nums[currentIndex];
                int preIndex = currentIndex - gap;
                // 向后挪位置
                while( preIndex >= groupStartIndex && currentNumber < nums[preIndex] ) {
                    nums[preIndex + gap] = nums[preIndex];
                    preIndex -= gap;
                }
                // currentNumber 找到了自己的位置，坐下
                nums[preIndex + gap] = currentNumber;
            }
        }
    }
}

void shellSort2(vector<int>& nums) {
    //// 间隔序列，在希尔排序中我们称之为增量序列
    for( int gap = nums.size() / 2; gap > 0; gap /= 2 ) {
        // 从 gap 开始，按照顺序将每个元素依次向前插入自己所在的组
        for( int i = gap; i < nums.size(); i++ ) {
            // currentNumber 站起来，开始找位置
            int currentNumber = nums[i];
            // 该组前一个数字的索引
            int preIndex = i - gap;
            while( preIndex >= 0 &&  currentNumber < nums[preIndex] ) {
                // 向后挪位置
                nums[preIndex + gap] = nums[preIndex];
                preIndex -= gap;
            }
            // currentNumber 找到了自己的位置，坐下
            nums[preIndex + gap] = currentNumber;
        }
    }
}

//调整大顶堆，，第三个参数表示剩余未排序的数字的数量，也就是剩余堆的大小
void maxHeapify(vector<int>& nums, int i, int heapSize) {
    int left = 2*i + 1;    // 左子结点下标
    int right = left + 1;  // 右子结点下标
    int largest = i;       // 记录根结点、左子树结点、右子树结点三者中的最大值下标
    
    // 与左子树结点比较
    if( left < heapSize && nums[left] > nums[largest] ) {
        largest = left;
    }
    // 与右子树结点比较
    if( right < heapSize && nums[right] > nums[largest] ) {
        largest = right;
    }
    if( largest != i ) {
        // 将最大值交换为根结点
        swap( nums[largest], nums[i] );
        // 再次调整交换数字后的大顶堆
        maxHeapify(nums,largest,heapSize);
    }
}

void buildMaxHeap(vector<int>& nums) {
    // 从最后一个非叶子结点开始调整大顶堆，最后一个非叶子结点的下标就是 arr.length / 2-1
    for( int i = nums.size() / 2 - 1; i>=0; i-- ) {
        maxHeapify(nums, i, nums.size());
    }
}

void heapSort(vector<int>& nums) {
    //构建初始大顶堆
    buildMaxHeap(nums);
    for( int i = nums.size() - 1; i > 0; i-- ) {
        //将最大值放到最后
        swap( nums[0], nums[i] );
        //调整剩余数组，使其满足大顶堆
        maxHeapify(nums, 0, i);
    }
}

//最简单的分区
// 将 arr 从 start 到 end 分区，左边区域比基数小，右边区域比基数大，然后返回中间值的下标
int partition1(vector<int>& nums, int start, int end) {
    // 取第一个数为基数
    int pivot = nums[start];
    // 从第二个数开始分区
    int left = start + 1;
    // 右边界
    int right = end;
    // left、right 相遇时退出循环
    while( left < right ) {
        // 找到第一个大于基数的位置
        while( left < right && nums[left] <= pivot ) left++;
        // 交换这两个数，使得左边分区都小于或等于基数，右边分区大于或等于基数
        if( left != right ) {
            swap( nums[left], nums[right] );
            right--;
        }
    }
    // 如果 left 和 right 相等，单独比较 arr[right] 和 pivot
    if( left == right && nums[right] > pivot ) right--;
    // 将基数和中间数交换
    if( right != start ) swap( nums[start],nums[right] );
    // 返回中间值的下标
    return right;
}

//双指针分区
// 将 arr 从 start 到 end 分区，左边区域比基数小，右边区域比基数大，然后返回中间值的下标
int partition2(vector<int>& nums, int start, int end) {
    // 取第一个数为基数
    int pivot = nums[start];
    // 从第二个数开始分区
    int left = start + 1;
    // 右边界
    int right = end;
    // left、right 相遇时退出循环
    while( left < right ) {
        // 找到第一个大于基数的位置
        while( left < right && nums[left] <= pivot ) left++;
        while( left < right && nums[right] >= pivot ) right--;
        // 交换这两个数，使得左边分区都小于或等于基数，右边分区大于或等于基数
        if( left < right ) {
            swap( nums[left], nums[right] );
            left++;
            right--;
        }
    }
    // 如果 left 和 right 相等，单独比较 arr[right] 和 pivot
    if( left == right && nums[right] > pivot ) right--;
    // 将基数和中间数交换
    swap( nums[start],nums[right] );
    // 返回中间值的下标
    return right;
}

void quickSort(vector<int>& nums, int start, int end) {
    // 如果区域内的数字少于 2 个，退出递归
    if( start >= end ) return;
    // 将数组分区，并获得中间值的下标
    int middle = partition2(nums, start ,end);
    // 对左边区域快速排序
    quickSort(nums,start,middle - 1);
    // 对右边区域快速排序
    quickSort(nums,middle + 1, end);
}

void quickSort(vector<int>& nums) {
    quickSort(nums, 0, nums.size() - 1);
}

// 将 result 的 [start, middle] 和 [middle + 1, end] 区间合并
void merge(vector<int>& nums, int start, int end, vector<int>& result) {
    int middle = start + ( end - start ) / 2;
     // 数组 1 的首尾位置
    int start1 = start;
    int end1 = middle;
    // 数组 2 的首尾位置
    int start2 = middle + 1;
    int end2 = end;
    // 用来遍历数组的指针
    int index1 = start1;
    int index2 = start2;
    // 结果数组的指针
    int resultIndex = start1;

    while (index1 <= end1 && index2 <= end2) {
        if (nums[index1] <= nums[index2]) {
            result[resultIndex++] = nums[index1++];
        } else {
            result[resultIndex++] = nums[index2++];
        }
    }
    // 将剩余数字补到结果数组之后
    while (index1 <= end1) {
        result[resultIndex++] = nums[index1++];
    }
    while (index2 <= end2) {
        result[resultIndex++] = nums[index2++];
    }
    // 将 result 操作区间的数字拷贝到 arr 数组中，以便下次比较
    for (int i = start; i <= end; i++) {
        nums[i] = result[i];
    }
}

// 对 arr 的 [start, end] 区间归并排序
void mergeSort(vector<int>& nums, int start, int end, vector<int>& result) {
     // 只剩下一个数字，停止拆分
    if( start == end ) return;
    int middle = start + ( end - start ) / 2;
    // 拆分左边区域，并将归并排序的结果保存到 result 的 [start, middle] 区间
    mergeSort(nums,start,middle, result);
    // 拆分右边区域，并将归并排序的结果保存到 result 的 [middle + 1, end] 区间
    mergeSort(nums,middle + 1, end, result);
    // 合并左右区域到 result 的 [start, end] 区间
    merge(nums,start,end,result);
}

void mergeSort(vector<int>& nums) {
    if( nums.size() == 0 ) return;
    vector<int> result = vector<int>(nums.size());
    mergeSort(nums,0,nums.size() - 1,result);
}



int main() {
    vector<int> arr = {9,2,5,6,3,4,55,3,2,1};
    mergeSort(arr);
    for( auto n : arr )
        cout << n << " ";
    cout << endl;
    return 0;
}