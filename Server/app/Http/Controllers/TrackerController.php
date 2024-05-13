<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Models\Tracker;
use App\Models\Position;
use Illuminate\Support\Carbon;

class TrackerController extends Controller
{
    public function create(Request $request)
    {
        // Validate the incoming request data
        $request->validate([
            'name' => 'required|string',
        ]);

        // Create a new tracker
        $tracker = Tracker::create([
            'user_id' => $request->user()->id,
            'name' => $request->name,
        ]);

        return response()->json(['tracker' => $tracker], 201);
    }

    public function getTrackers(Request $request)
    {
        $trackers = Tracker::where('user_id', $request->user()->id)->get();
        return response(['trackers' => $trackers], 200);
    }

    public function addPosition(Request $request, $trackerId)
    {
        // Validate the incoming request data
        $request->validate([
            'latitude' => 'required',
            'longitude' => 'required',
            'date' => 'required|date_format:Y-m-d H:i:s'
        ]);

        // Find the tracker
        $tracker = Tracker::findOrFail($trackerId);

        // Add a new position to the tracker
        $position = $tracker->positions()->create([
            'latitude' => $request->latitude,
            'longitude' => $request->longitude,
            'date'=> $request->date,
        ]);

        return response()->json(['position' => $position], 201);
    }

    // get last Position of tracker
    public function getLastPositions(Request $request)
    {
        $trackers = Tracker::where('user_id', $request->user()->id)->get()->toArray();
        $positions = Position::where('tracker_id', array_column($trackers, 'id'))->get()->last();
        return response()->json($positions);
    }

    // should return all positions of tracker in given time
    // app wasn't ready, so it only returns last
    // to restore just use the first return and delete the second
    public function getPositions(Request $request, $trackerId)
    {
        // Find the tracker
        $tracker = Tracker::findOrFail($trackerId);

        // check if time parameter is specified
        if ($request->has('time'))
        {
            $positions = $tracker->positions->where('date', '>', Carbon::now('Europe/Vienna')->subHours($request->query('time')));
        }
        else
        {
            $positions = $tracker->positions;
        }

        return response()->json(['positions' => $positions]);
        //return response()->json($positions->last());
    }
}
