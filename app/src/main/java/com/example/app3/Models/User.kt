package com.example.app3.Models

import com.google.firebase.database.Exclude

data class User(var ItemDesc: String ?= null) //@get:Exclude var isDeleted: Boolean = false )
//{
//    override fun equals(other: Any?): Boolean{
//        return if(other is User){
//            other.ItemDesc == ItemDesc
//        }else false
//    }
//
//    override fun hashCode(): Int {
//        var result = ItemDesc?.hashCode() ?: 0
//        result = 31 * result + (Price?.hashCode() ?: 0)
//        result = 31 * result + (Weight?.hashCode() ?: 0)
//        result = 31 * result + (timestamp?.hashCode() ?: 0)
//        result = 31 * result + isDeleted.hashCode()
//        return result
//    }
//
//}

