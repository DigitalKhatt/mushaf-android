package org.digitalkhatt.mushaf
import android.os.Bundle
import android.view.Menu
import android.view.MenuInflater
import android.view.MenuItem
import android.widget.CompoundButton
import android.widget.Switch
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.LinearLayoutManager
import org.digitalkhatt.mushaf.data.QuranPage
import org.digitalkhatt.mushaf.databinding.ActivityMainBinding


class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val pageAdapter = PageAdapter { page -> adapterOnClick(page) }

        val list = mutableListOf<QuranPage>()

        for (i in 0..603) {
            list.add(QuranPage(i))
        }

        pageAdapter.submitList(list)

        binding = ActivityMainBinding.inflate(layoutInflater)

        val layoutManager = LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false)

        binding.recyclerView.layoutManager = layoutManager;

        val dividerItemDecoration = DividerItemDecoration(
            binding.recyclerView.getContext(),
            layoutManager.getOrientation()
        )
        binding.recyclerView.addItemDecoration(dividerItemDecoration)

        binding.recyclerView.adapter = pageAdapter
        setContentView(binding.root)
    }

    private fun adapterOnClick(page: QuranPage) {

    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        val inflater: MenuInflater = menuInflater
        inflater.inflate(R.menu.mainmenu, menu)

        var menuItem = menu.findItem(R.id.justify)
        val justifySwitch = menuItem.actionView as Switch?
        justifySwitch?.setChecked(true)

        menuItem = menu.findItem(R.id.tajweed)
        val tajeweedSwitch = menuItem.actionView as Switch?
        tajeweedSwitch?.setChecked(true)

        justifySwitch!!.setOnCheckedChangeListener { buttonView: CompoundButton?, isChecked: Boolean ->
            MushafApp.instance.justify = !MushafApp.instance.justify
            binding.recyclerView.adapter?.notifyDataSetChanged()
        }

        tajeweedSwitch!!.setOnCheckedChangeListener { buttonView: CompoundButton?, isChecked: Boolean ->
            MushafApp.instance.tajweed = !MushafApp.instance.tajweed
            binding.recyclerView.adapter?.notifyDataSetChanged()
        }

        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        // Handle item selection
        return when (item.itemId) {
            R.id.justify -> {
                //newGame()
                true
            }
            R.id.tajweed -> {
                //showHelp()
                true
            }
            else -> super.onOptionsItemSelected(item)
        }
    }

}