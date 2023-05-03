package com.example.app3

import android.os.Bundle
import android.widget.Toast
//import android.support.v4.app.Fragment
//import android.support.v7.app.AppCompatActivity
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.RecyclerView
import com.example.app3.Adapter.MyAdapter
import com.example.app3.Models.UserViewModel
import com.google.android.material.snackbar.Snackbar


class MainActivity : AppCompatActivity(){
    override fun onCreate(savedInstanceState: Bundle?){
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)


        replaceFragment(Home())



    }


    private fun replaceFragment(fragment: Fragment){
        val fragmentManager = supportFragmentManager
        val fragmentTransaction = fragmentManager.beginTransaction()
        fragmentTransaction.replace(R.id.frame_layout, fragment)
        fragmentTransaction.commit()

    }



}