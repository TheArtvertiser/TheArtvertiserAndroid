package cc.openframeworks;

import java.util.List;
import java.util.Locale;

import android.location.Address;
import android.location.Geocoder;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup.MarginLayoutParams;
import cc.openframeworks.androidFerns.R;

import com.google.android.maps.GeoPoint;
import com.google.android.maps.MapView;

public class OFAndroidGoogleMaps extends OFAndroidObject {
	
	public OFAndroidGoogleMaps(){
		geocoder = new Geocoder(activity,new Locale("EN"));
	}
	
	public void showMap(int _x, int _y, int _w, int _h){
		final int x = _x;
		final int y = _y;
		final int w = _w;
		final int h = _h;
		activity.runOnUiThread(new Runnable(){
			public void run() {
				MapView mapView = (MapView)activity.findViewById(R.id.mapview);
				mapView.setBuiltInZoomControls(true);
				MarginLayoutParams params = (MarginLayoutParams) mapView.getLayoutParams();
				params.width = w;
				params.height = h;
				params.setMargins(x, y, 0,0);
				mapView.setLayoutParams(params);
				mapView.requestLayout();
				//mapView.setPadding(x, y, 0, 0);
				mapView.setVisibility(View.VISIBLE);
			}
		});
	}
	
	public void hideMap(){
		activity.runOnUiThread(new Runnable(){
			public void run() {
				activity.findViewById(R.id.mapview).setVisibility(View.GONE);
			}
		});
	}
	
	public void setLocation(double latitude, double longitude){
		MapView mapView = (MapView)activity.findViewById(R.id.mapview);
		mapView.getController().animateTo(new GeoPoint((int)(latitude*1e6),(int)(longitude*1e6)));
	}
	
	public String getAddress(double latitude, double longitude){
		List<Address> locations;
		try {
			locations = geocoder.getFromLocation(latitude, longitude, 1);
			if(locations.size()>0){
				return locations.get(0).getAddressLine(0);
			}else{
				return "";
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			Log.e("OF","Cannot get address from location",e);
			return "";
		}
	}
	
	public String getCountry(double latitude, double longitude){
		List<Address> locations;
		try {
			locations = geocoder.getFromLocation(latitude, longitude, 1);
			if(locations.size()>0){
				return locations.get(0).getCountryName();
			}else{
				return "";
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			Log.e("OF","Cannot get address from location",e);
			return "";
		}
	}
	
	public String getCity(double latitude, double longitude){
		List<Address> locations;
		try {
			locations = geocoder.getFromLocation(latitude, longitude, 1);
			if(locations.size()>0){
				return locations.get(0).getLocality();
			}else{
				return "";
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			Log.e("OF","Cannot get address from location",e);
			return "";
		}
	}
	
	public void setZoom(int zoom){
		MapView mapView = (MapView)activity.findViewById(R.id.mapview);
		mapView.getController().setZoom(zoom);
	}
	
	@Override
	protected void appPause() {
		// TODO Auto-generated method stub
		
	}

	@Override
	protected void appResume() {
		// TODO Auto-generated method stub
		
	}

	@Override
	protected void appStop() {
		// TODO Auto-generated method stub
		
	}

	private Geocoder geocoder;
	private OFAndroidGPS gps;
}
