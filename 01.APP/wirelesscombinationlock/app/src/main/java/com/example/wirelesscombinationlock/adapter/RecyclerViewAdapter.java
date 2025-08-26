package com.example.wirelesscombinationlock.adapter;

import android.content.Context;
import android.net.MailTo;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.recyclerview.widget.RecyclerView;

import com.example.wirelesscombinationlock.R;
import com.example.wirelesscombinationlock.dao.User;
import com.example.wirelesscombinationlock.dao.UserDao;
import com.example.wirelesscombinationlock.databinding.UserManageListViewBinding;
import com.example.wirelesscombinationlock.utils.Common;
import com.example.wirelesscombinationlock.utils.MToast;


import java.util.ArrayList;
import java.util.List;

public class RecyclerViewAdapter extends RecyclerView.Adapter<RecyclerViewAdapter.ViewHolder> {
    private List<Object> dataList;
    private Context context;
    private UserDao dao;

    public RecyclerViewAdapter(List<Object> dataList, Context context) {
        this.dataList = dataList;
        this.context = context;
        dao = new UserDao(context);
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.user_manage_list_view, parent, false);
        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {

        User user = (User) dataList.get(position);
        holder.userName.setText(user.getName());
        holder.userLock.setText(user.getLock() != null && !user.getLock().isEmpty() ? user.getLock() : "无");
        holder.creatTime.setText(user.getCreateDateTime());
        holder.updateBtn.setOnClickListener(view -> {
            // 创建对话框构建器
            AlertDialog.Builder builder = new AlertDialog.Builder(context);
            // 获取布局
            View view2 = View.inflate(context, R.layout.update_user, null);
            // 获取布局中的控件
            final EditText name = (EditText) view2.findViewById(R.id.inputNameEdit);
            final EditText password = (EditText) view2.findViewById(R.id.inputPasswordEdit);
            final EditText age = (EditText) view2.findViewById(R.id.inputAgeEdit);
            final EditText location = (EditText) view2.findViewById(R.id.inputLocationEdit);
            final EditText phone = (EditText) view2.findViewById(R.id.inputPhoneEdit);
            final RadioGroup inputopen = (RadioGroup) view2.findViewById(R.id.inputopenRadio);
            final RadioGroup sex = (RadioGroup) view2.findViewById(R.id.inputSex);
            final EditText lockText = (EditText) view2.findViewById(R.id.inputLockEdit);
            final Button updateBtn = (Button) view2.findViewById(R.id.updateBtn);
            final Button cancelBtn = (Button) view2.findViewById(R.id.cancelBtn);
            name.setText(user.getName());
            phone.setText(user.getPhone());
            password.setText(user.getPassword());
            location.setText(user.getLocation());
            age.setText(user.getAge() + "");
            inputopen.check(user.getOpen() == 1 ? R.id.openRadiotrue : R.id.openRadioFalse);
            sex.check(user.getSex().equals("男") ? R.id.sexMan : R.id.sexWoman);
            lockText.setText(user.getLock() != null && !user.getLock().isEmpty() ? user.getLock() : "无");

            // 设置参数
            builder.setTitle("修改通用密码").setView(view2);
            // 创建对话框
            final AlertDialog alertDialog = builder.create();
            cancelBtn.setOnClickListener(view1 -> alertDialog.dismiss());
            updateBtn.setOnClickListener(v -> {
                String name1 = name.getText().toString();
                String age1 = age.getText().toString();
                String password1 = password.getText().toString();
                String location1 = location.getText().toString();
                String phone1 = phone.getText().toString();
                RadioButton sex_temp = view2.findViewById(sex.getCheckedRadioButtonId());
                RadioButton open_temp = view2.findViewById(inputopen.getCheckedRadioButtonId());
                String inputopen1 = open_temp.getText().toString();
                String sex1 = sex_temp.getText().toString();
                if (name1.isEmpty()) {
                    MToast.mToast(context, "用户名不能为空");
                    return;
                }
                if (age1.isEmpty()) {
                    MToast.mToast(context, "年龄不能为空");
                    return;
                }
                if (password1.isEmpty()) {
                    MToast.mToast(context, "密码不能为空");
                    return;
                }
                if (location1.isEmpty()) {
                    MToast.mToast(context, "籍贯不能为空");
                    return;
                }
                if (phone1.isEmpty()) {
                    MToast.mToast(context, "手机号不能为空");
                    return;
                }
                if (sex1.isEmpty()) {
                    MToast.mToast(context, "性别不能为空");
                    return;
                }

                List<Object> objects = dao.query(name1, "name");
                if (!user.getName().equals(name1) && objects.size() != 0) {
                    MToast.mToast(context, "已有该用户名");
                    return;
                }
                user.setSex(sex1);
                user.setPhone(phone1);
                user.setName(name1);
                user.setPassword(password1);
                user.setLocation(location1);
                user.setAge(Integer.parseInt(age1));
                user.setLock(!lockText.getText().toString().equals("无") ? lockText.getText().toString() : null);
                user.setOpen(inputopen1.equals("开锁") ? 1 : 0);
                dao.update(user);
//                List<Object> objects1 = dao.query();
//                int i = 0;
//                for (; i < objects1.size(); i++) {
//                    User temp = (User) objects1.get(i);
//                    if (temp.getPer() == 1) break;
//                }
//                dataList = objects1;
                notifyDataSetChanged();
                alertDialog.dismiss();// 对话框消失
            });
            alertDialog.show();
        });
        holder.deleteBtn.setOnClickListener(view -> {
            dao.delete(user.getUid() + "");
            List<Object> objects = dao.query();
            int i = 0;
            for (; i < objects.size(); i++) {
                User temp = (User) objects.get(i);
                if (temp.getPer() == 1) break;
            }
            if (i != objects.size()) {
                objects.remove(i);
            }
            objects.remove(user);
            dataList = objects;
            notifyDataSetChanged();
        });
    }

    @Override
    public int getItemCount() {
        return dataList.size();
    }

    class ViewHolder extends RecyclerView.ViewHolder {
        TextView userName, userLock, creatTime;
        Button updateBtn, deleteBtn;

        public ViewHolder(@NonNull View itemView) {
            super(itemView);
            userName = itemView.findViewById(R.id.userName);
            updateBtn = itemView.findViewById(R.id.updateBtn);
            userLock = itemView.findViewById(R.id.userLock);
            creatTime = itemView.findViewById(R.id.creatTime);
            deleteBtn = itemView.findViewById(R.id.deleteBtn);
        }
    }
}
