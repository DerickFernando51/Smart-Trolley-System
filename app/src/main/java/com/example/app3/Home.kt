package com.example.app3

import android.app.AlertDialog
import android.content.DialogInterface
import android.content.Intent
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import androidx.recyclerview.widget.ItemTouchHelper
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.example.app3.Adapter.MyAdapter
import com.example.app3.Models.UserViewModel

// TODO: Rename parameter arguments, choose names that match
// the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
private const val ARG_PARAM1 = "param1"
private const val ARG_PARAM2 = "param2"


/**
 * A simple [Fragment] subclass.
 * Use the [Home.newInstance] factory method to
 * create an instance of this fragment.
 */

private lateinit var viewModel : UserViewModel
private lateinit var userRecyclerView : RecyclerView
internal lateinit var adapter : MyAdapter

class Home : Fragment() {
    // TODO: Rename and change types of parameters
    private var param1: String? = null
    private var param2: String? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            param1 = it.getString(ARG_PARAM1)
            param2 = it.getString(ARG_PARAM2)
        }


    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_home, container, false)
    }

    companion object {
        /**
         * Use this factory method to create a new instance of
         * this fragment using the provided parameters.
         *
         * @param param1 Parameter 1.
         * @param param2 Parameter 2.
         * @return A new instance of fragment Home.
         */
        // TODO: Rename and change types and number of parameters
        @JvmStatic
        fun newInstance(param1: String, param2: String) =
            Home().apply {
                arguments = Bundle().apply {
                    putString(ARG_PARAM1, param1)
                    putString(ARG_PARAM2, param2)
                }
            }
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        userRecyclerView = view.findViewById(R.id.recyclerView)
        userRecyclerView.layoutManager = LinearLayoutManager(context)
        userRecyclerView.setHasFixedSize(true)
        adapter = MyAdapter()
        userRecyclerView.adapter = adapter

        viewModel = ViewModelProvider(this).get(UserViewModel::class.java)

        viewModel.allUsers.observe(viewLifecycleOwner, Observer {
            adapter.updateUserList(it)


        })

        //adapter.onItemClick = {

            ///adapter.deleteItem()

                // build alert dialog
//                val dialogBuilder = AlertDialog.Builder(this.context)
//
//                // set message of alert dialog
//                dialogBuilder.setMessage("Do you want to close this application ?")
//                    // if the dialog is cancelable
//                    .setCancelable(false)
//                    // positive button text and action
//                    .setPositiveButton("Proceed", DialogInterface.OnClickListener {
//                            dialog, id -> adapter.deleteItem(it)
//                    })
//                    // negative button text and action
//                    .setNegativeButton("Cancel", DialogInterface.OnClickListener {
//                            dialog, id -> dialog.cancel()
//                    })
//
//                // create dialog box
//                val alert = dialogBuilder.create()
//                // set title for alert dialog box
//                alert.setTitle("AlertDialogExample")
//                // show alert dialog
//                alert.show()
            //}

            //Toast.makeText(applicationContext,"this is toast message", Toast.LENGTH_SHORT).show()
        }




}


