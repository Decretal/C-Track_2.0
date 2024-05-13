<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Position extends Model
{
    use HasFactory;
    public $timestamps = false;
    protected $fillable = ['tracker_id', 'latitude', 'longitude', 'date']; // Add other attributes as needed

    public function tracker()
    {
        return $this->belongsTo(Tracker::class);
    }
}
