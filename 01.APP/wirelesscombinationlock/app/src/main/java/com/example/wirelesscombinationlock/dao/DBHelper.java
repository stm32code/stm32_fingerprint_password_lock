package com.example.wirelesscombinationlock.dao;

import android.content.ContentValues;
import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import androidx.annotation.Nullable;

import com.example.wirelesscombinationlock.utils.TimeCycle;

public class DBHelper extends SQLiteOpenHelper {
    private static final String DB_NAME = "my.db";//数据库名称
    private static final int VERSION = 1;//数据库版本 这里定死为1

    public DBHelper(@Nullable Context context) {
        super(context, DB_NAME, null, VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase sqLiteDatabase) {
        String sql = "create table `user` (" +
                "`uid` INTEGER primary key autoincrement," +
                "`name` VARCHAR(255)," +
                "`password` VARCHAR(255)," +
                "`age` INTEGER," +
                "`sex` VARCHAR(255)," +
                "`location` VARCHAR(255)," +
                "`phone` VARCHAR(255)," +
                "`open` INTEGER, " +
                "`lock` VARCHAR(255)," +
                "`per` INTEGER," +
                "`createDateTime` VARCHAR(255))";
        sqLiteDatabase.execSQL(sql);//执行sql语句，user表

        ContentValues data = new ContentValues();
        data.put("name", "admin");
        data.put("age", 0);
        data.put("sex", "未知");
        data.put("location", "");
        data.put("phone", "123456");
        data.put("per", 1);
        data.put("open", 1);
        data.put("lock", "0000");
        data.put("password", "admin");
        data.put("createDateTime", TimeCycle.getDateTime());
        sqLiteDatabase.insert("user", null, data);

    }

    @Override
    public void onUpgrade(SQLiteDatabase sqLiteDatabase, int i, int i1) {

    }
}