package com.example.wirelesscombinationlock;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.os.Bundle;
import android.view.MenuItem;

import com.example.wirelesscombinationlock.adapter.RecyclerViewAdapter;
import com.example.wirelesscombinationlock.dao.User;
import com.example.wirelesscombinationlock.dao.UserDao;
import com.example.wirelesscombinationlock.databinding.ActivityUserManageBinding;
import com.gyf.immersionbar.ImmersionBar;

import java.util.List;
import java.util.Objects;

public class UserManageActivity extends AppCompatActivity {
    private ActivityUserManageBinding binding;
    private UserDao dao;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityUserManageBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        dao = new UserDao(this);
        setSupportActionBar(binding.toolbar);
        binding.toolbarLayout.setTitle("管理用户");
        ImmersionBar.with(this).init();
        Objects.requireNonNull(getSupportActionBar()).setDisplayHomeAsUpEnabled(true);//添加默认的返回图标
        getSupportActionBar().setHomeButtonEnabled(true); //设置返回键可用
        initView();
    }

    private void initView() {
        List<Object> objects = dao.query();
        int i = 0;
        for (; i < objects.size(); i++) {
            User user = (User) objects.get(i);
            if (user.getPer() == 1) break;
        }
        if (i != objects.size()) {
            objects.remove(i);
        }

        LinearLayoutManager layoutManager = new LinearLayoutManager(this);
        layoutManager.setOrientation(LinearLayoutManager.VERTICAL);
        binding.userManageRecyclerView.setAdapter(new RecyclerViewAdapter(objects, this));
        binding.userManageRecyclerView.setLayoutManager(layoutManager);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        finish();
        return super.onOptionsItemSelected(item);
    }
}