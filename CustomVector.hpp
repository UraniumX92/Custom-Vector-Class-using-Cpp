#include<iostream>

template<class Type>
class Cvector
{
    protected:
        int capacity;
        int current = 0;
        Type * v_array;
        static constexpr double multiplier = 1.5;

        /*
        Takes 2 pointers of template Type. 1. newptr ,2. oldptr
        copies the content from oldptr array into the newptr
        */
        void copy_array(Type* newptr,Type* oldptr)
        {
            for(int i=0;i<current;i++)
            {
                newptr[i] = oldptr[i];
            }
        }

        /*
        Increases the capacity of array by 1
        allocates new memory of size capacity and copies the array content into new array
        deletes the old allocated memory and points the `array` to new allocated memory 
        */
        void increase_capacity()
        {
            capacity *= multiplier;
            Type* tempArr = new Type[capacity];
            copy_array(tempArr,v_array);
            delete[] v_array;
            v_array = tempArr;
        }

        /*
        Takes an integer `index` and `shift` and shifts all the elements in array including index to right
        creating space for `shift` number of elements in between. Default value for `shift` is 1.
        Note: `current` should be updated manually after using this method.
        */
        void shift_right(int index,int shift=1)
        {
            // handling of unexpected value passed in `shift` parameter
            if(shift<=0)
            {
                if(shift<0)
                {
                    std::out_of_range err("Shift cannot be negative");
                    throw err;
                }
                else
                {
                    return;
                }
            }
            // Increasing the capacity until we have enough space to shift array.
            while((current+shift)>=capacity)
            {
                increase_capacity();
            }
            int tempCurrent = current;
            tempCurrent += shift-1;            
            for(int i=tempCurrent;i>=index;i--)
            {
                if(i-shift>=0)
                {
                    v_array[i] = v_array[i-shift];
                }
                else
                {
                    break;
                }
            }
        }

        /*
        Takes an integer `index` and `shift` and shifts all the elements in array including index to left
        removing `shift` number of elements in between. Default value for `shift` is 1.
        Note: `current` should be updated manually after using this method.
        */
        void shift_left(int index,int shift=1)
        {
            // handling of unexpected value passed in `shift` parameter
            if(shift<=0)
            {
                if(shift<0)
                {
                    std::out_of_range err("Shift cannot be negative");
                    throw err;
                }
                else
                {
                    return;
                }
            }
            
            for(int i=index;i<current;i++)
            {
                if(i+shift<current)
                {
                    v_array[i] = v_array[i+shift];
                }
                else
                {
                    break;
                }
            }
        }
        
    public:
        /*
        Constructor: Takes an int vector_size and allocates the memory blocks for array
        default value for vector_size is 10
        */
        Cvector(int vector_size=10)
        {
            v_array = new Type[vector_size];
            capacity = vector_size;
        }

        /*
        Destructor: Deletes the allocated memory of vector while object is out of usage.
        */
        ~Cvector()
        {
            delete[] v_array;
        }

        /*
        Overloading [] operator to make the class object to be used as array
        if the index accessed is greater than `current`, then `current` gets updated to `index+1`
        */
        Type& operator [] (int index)
        {
            if(index>=capacity)
            {
                std::out_of_range err("Array index out of range");
                throw err;
            }
            else
            {
                current = (index<current)? current : index+1;
                return v_array[index];
            }
        }

        Type get(int index)
        {
            if(index >= capacity)
            {
                std::out_of_range err("Array index out of range");
                throw err;
            }
            else
            {
                return v_array[index];
            }
        }

        void set(int index,Type data)
        {
            if(index >= current)
            {
                std::string error_prompt = "Cannot set value at index ("+std::to_string(index)+") current maximum index ("+std::to_string(current-1)+")\n";
                error_prompt += "use `insert()` method instead.";
                std::out_of_range err(error_prompt);
                throw err;
            }
            else
            {
                v_array[index] = data;
            }
        }

        /*
        Removes the last element(s) from array
        */
        void pop(int num=1)
        {
            current-=num;
        }

        /*
        Inserts the data at last (used) index of array
        */
        void push(Type data)
        {
            if(current==capacity)
            {
                increase_capacity();
            }
            v_array[current] = data;
            current++;
        }

        /*
        Takes index and data
        inserts data at the given index in array
        can insert data within the `capacity` of the array.
        */
        void insert(int index,Type data)
        {
            if(index >= capacity)
            {
                std::out_of_range err("Array index out of range");
                throw err;
            }
            else
            {
                shift_right(index);
                v_array[index] = data;
                if(index < current)
                {
                    current++;
                }
                else
                {
                    current = index+1;
                }
            }
        }

        /*
        Takes the int `index` , `num` and removes the `num` number of values starting from `index` and left-shifts the entire array to maintain order
        default value of `num` is 1.
        */
        void del(int index,int num=1)
        {
            if(index>=current)
            {
                std::string error_prompt = "Requested index (" + std::to_string(index) + ") is out of range. current max index is: " + std::to_string(current-1);
                std::out_of_range err(error_prompt);
                throw err;
            }
            shift_left(index,num);
            if(index+num<=current)
            {
                current -= num;
            }
            else
            {
                current -= current-index;
            }
        }

        /*
        Takes arrayPtr, arraySize , vectIndex
        and copies the content of given array into vector array starting from index : vectIndex (default value for vectIndex is 0)
        shifts the remaining items of array to the right of vector array.
        */
        void merge_array(Type* arrayPtr,int arraySize,int vectIndex=0)
        {
            int v_start = vectIndex;
            int v_end = vectIndex + arraySize;

            shift_right(vectIndex,arraySize);
            
            // Copying the content from given array into vector
            for(int i=v_start,j=0; i<v_end; i++,j++)
            {
                v_array[i] = arrayPtr[j];
            }
            current += arraySize;
        }

        /*
        Traverses through the used memory blocks and displays the content of array in that memory
        */
        void display()
        {
            for(int i=0;i<current;i++)
            {
                std::cout << "Index "<< i << " : "<< v_array[i] << std::endl;
            }
        }

        /*
        Traverses through the entire allocated memory and displays the array from 0th index to capacity
        WARNING: Might display garbage values as well if all the capacity isn't used.
        */
        void display_all()
        {
            for(int i=0;i<capacity;i++)
            {
                std::cout << "Index "<< i << " : "<< v_array[i] << std::endl;
            }
        }

        /*
        Get the number of memory blocks of array which are currently in use.
        */
        int get_current()
        {
            return current;
        }

        /*
        Get the total number of memory blocks of array which are currently available to use
        Note: The capacity will be auto updated accordingly when inserting the values in array
        */
        int get_capacity()
        {
            return capacity;
        }

};