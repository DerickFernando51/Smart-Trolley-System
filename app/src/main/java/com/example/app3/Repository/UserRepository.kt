package com.example.app3.Repository

import androidx.lifecycle.MutableLiveData
import com.example.app3.Models.User
import com.google.firebase.database.*
import com.google.firebase.database.ktx.database
import com.google.firebase.ktx.Firebase

class UserRepository {

    private val databaseReference : DatabaseReference = FirebaseDatabase.getInstance().getReference("Smart_Cart_Wishlist")

    //private val databaseReference = Firebase.database.getReference("Smart_Cart_Products")



    @Volatile private var INSTANCE : UserRepository ?= null

    fun getInstance() : UserRepository{
        return INSTANCE ?: synchronized(this){
            val instance = UserRepository()
            INSTANCE = instance
            instance
        }
    }

    fun loadUsers(userList : MutableLiveData<List<User>>){
        databaseReference.addValueEventListener(object : ValueEventListener{
            override fun onDataChange(snapshot: DataSnapshot) {

                try{

                    val _userList : List<User> = snapshot.children.map {dataSnapshot ->
                        dataSnapshot.getValue(User::class.java)!!
                    }
                    userList.postValue(_userList)

                }catch(e: Exception){

                }

            }

            override fun onCancelled(error: DatabaseError) {

            }

        })
    }
}