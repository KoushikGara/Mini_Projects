package com.monitoring.app.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import com.monitoring.app.services.MonitoringService;


@RestController
@RequestMapping("/api/")
public class MonitoringController 
{
	@Autowired
	private MonitoringService monitoringService;
	
	@GetMapping("checkstatusRestTemplate/")
	public ResponseEntity<?>getStatus()
	{
		String resourceUrl = "http://localhost:9090/api/users/";
		String result = monitoringService.checkStatus(resourceUrl);
		return ResponseEntity.status(HttpStatus.OK).body(result);
	}
}
