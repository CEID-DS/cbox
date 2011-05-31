package com.cbox.provider;

import android.content.ContentProvider;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.net.Uri;

public class CboxProvider extends ContentProvider{

	private SQLiteDatabase sqlDB;
	private DatabaseHelper dbHelper;
	private static final String DATABASE_NAME = "cBox.db";
	private static final int DATABASE_VERSION = 1;
	private static final String SERVICE_TABLE = "Service";
	
	private static class DatabaseHelper extends SQLiteOpenHelper{

		public DatabaseHelper(Context context) {
			super(context,DATABASE_NAME,null,DATABASE_VERSION);
		}

		@Override
		public void onCreate(SQLiteDatabase db) {
			db.execSQL("Create table "+SERVICE_TABLE+"(_id integer primary key autoincrement,"+
					Services.Service.hostname+" text,"+
					Services.Service.serviceType+" text,"+
					Services.Service.protocol+" text,"+
					Services.Service.cinterface+" text"+
					Services.Service.port+" integer"+
					Services.Service.txtdata+" text"+
					Services.Service.ttl +" text"+
					Services.Service.advertised +" integer"+
					Services.Service.questioned +" integer)");
		}

		@Override
		public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
			db.execSQL("drop table if exists "+SERVICE_TABLE);
			onCreate(db);
		}
		
	}
	
	
	
	@Override
	public int delete(Uri uri, String selection, String[] selectionArgs) {	
		return 0;
	}

	@Override
	public String getType(Uri uri) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Uri insert(Uri uri, ContentValues values) {
		sqlDB = dbHelper.getWritableDatabase();
		long rowId = sqlDB.insert(SERVICE_TABLE,"", values);
		if(rowId>0){
			Uri rowUri = ContentUris.appendId(Services.Service.CONTENT_URI.buildUpon(),rowId).build();
			getContext().getContentResolver().notifyChange(rowUri,null);
			return rowUri;
		}
		return null;
	}

	@Override
	public boolean onCreate() {
		dbHelper = new DatabaseHelper(getContext());
		return (dbHelper == null)? false:true;
	}

	@Override
	public Cursor query(Uri uri, String[] projection, String selection,
			String[] selectionArgs, String sortOrder) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public int update(Uri uri, ContentValues values, String selection,
			String[] selectionArgs) {
		// TODO Auto-generated method stub
		return 0;
	}
	
}
