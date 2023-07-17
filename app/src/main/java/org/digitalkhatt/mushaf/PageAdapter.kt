package org.digitalkhatt.mushaf

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import androidx.recyclerview.widget.DiffUtil
import androidx.recyclerview.widget.ListAdapter
import androidx.recyclerview.widget.RecyclerView
import org.digitalkhatt.mushaf.data.QuranPage

class PageAdapter(private val onClick: (QuranPage) -> Unit) :
    ListAdapter<QuranPage, PageAdapter.PageViewHolder>(PageDiffCallback) {

    class PageViewHolder(itemView: View, val onClick: (QuranPage) -> Unit) :
        RecyclerView.ViewHolder(itemView) {
        private val pageView: PageView = itemView.findViewById(R.id.pageView1)
        private var currentPage: QuranPage? = null

        init {
            itemView.setOnClickListener {
                currentPage?.let {
                    onClick(it)
                }
            }
        }

        /* Bind flower name and image. */
        fun bind(page: QuranPage) {
            currentPage = page

            pageView.setPageIndex(page.index)

        }
    }

    /* Creates and inflates view and return FlowerViewHolder. */
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): PageViewHolder {
        val view = LayoutInflater.from(parent.context)
            .inflate(R.layout.page_item, parent, false)
        return PageViewHolder(view, onClick)
    }

    /* Gets current flower and uses it to bind view. */
    override fun onBindViewHolder(holder: PageViewHolder, position: Int) {
        val page = getItem(position)
        holder.bind(page)

    }
}

object PageDiffCallback : DiffUtil.ItemCallback<QuranPage>() {
    override fun areItemsTheSame(oldItem: QuranPage, newItem: QuranPage): Boolean {
        return oldItem == newItem
    }

    override fun areContentsTheSame(oldItem: QuranPage, newItem: QuranPage): Boolean {
        return oldItem.index == newItem.index
    }
}