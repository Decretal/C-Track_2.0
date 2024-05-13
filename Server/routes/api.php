<?php

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Route;
use App\Http\Controllers\AuthController;
use App\Http\Controllers\TrackerController;
/*
|--------------------------------------------------------------------------
| API Routes
|--------------------------------------------------------------------------
|
| Here is where you can register API routes for your application. These
| routes are loaded by the RouteServiceProvider and all of them will
| be assigned to the "api" middleware group. Make something great!
|
*/

//account related routes
Route::post('/account/register', [AuthController::class, 'register']);
Route::post('/account/login', [AuthController::class, 'login']);
Route::middleware('auth:sanctum')->get('/account/revoke-keys', [AuthController::class,'revoke_keys']);
Route::middleware('auth:sanctum')->get('/account/user', function (Request $request) {
    return $request->user();
});



// Tracker related routes

// Create a tracker
Route::middleware('auth:sanctum')->post('/trackers', [TrackerController::class, 'create']);
Route::middleware('auth:sanctum')->get('/trackers', [TrackerController::class, 'getTrackers']);

// Add a position to a tracker
Route::post('/trackers/{trackerId}/positions', [TrackerController::class, 'addPosition']);

// Get positions for a tracker
Route::middleware('auth:sanctum')->get('/trackers/{trackerId}/positions', [TrackerController::class, 'getPositions']);
Route::middleware('auth:sanctum')->get('/trackers/{trackerId}/position', [TrackerController::class, 'getLastPositions']);