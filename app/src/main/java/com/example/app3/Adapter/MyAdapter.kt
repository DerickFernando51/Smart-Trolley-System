package com.example.app3.Adapter


import android.content.Intent
import android.view.LayoutInflater
import android.view.TextureView
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.example.app3.Home
import com.example.app3.Models.User
import com.example.app3.R
import java.util.*

class MyAdapter : RecyclerView.Adapter<MyAdapter.MyViewHolder>() {

   val userList = ArrayList<User>()

    //var onItemClick : ((User)-> Unit)? = null

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): MyViewHolder {
        val itemView = LayoutInflater.from(parent.context).inflate(
            R.layout.user_item,
            parent,false
        )
        return MyViewHolder(itemView)








    }

    override fun onBindViewHolder(holder: MyViewHolder, position: Int) {
        val currentItem = userList[position]
        holder.itemDesc.text = currentItem.ItemDesc
//        holder.price.text = currentItem.Price
//        holder.weight.text = currentItem.Weight
//        holder.timestamp.text = currentItem.timestamp

//        holder.itemView.setOnClickListener {
//            onItemClick?.invoke(currentItem)
//
//        }


    }

    override fun getItemCount(): Int {
        return userList.size
    }

    fun updateUserList(userList : List<User>){
        this.userList.clear()
        this.userList.addAll(userList)
        notifyDataSetChanged()
    }

//    fun deleteItem(position: Int){
//        this.userList.removeAt(position)
//        notifyDataSetChanged()
//    }



    class MyViewHolder(itemView : View) : RecyclerView.ViewHolder(itemView){
        val itemDesc : TextView = itemView.findViewById(R.id.itemDesc)
//        val price : TextView = itemView.findViewById(R.id.price)
//        val weight : TextView = itemView.findViewById(R.id.weight)
//        val timestamp : TextView = itemView.findViewById(R.id.timestamp)
        //val buttondlt : Button = itemView.findViewById(R.id.deletebtn)








    }
}