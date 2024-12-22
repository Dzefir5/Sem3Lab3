#pragma once

template <typename T>
T my_abs(T input){
    if(input<0){
        return (-1)*input;
    }
    return input;
}