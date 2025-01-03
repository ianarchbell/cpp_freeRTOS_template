/*
 * Task.cpp
 *  Based on this work for esp-idf and ported to Pico
 *  Created on: Mar 4, 2017
 *      Authors: kolban
 *               archbell
 */

#include "FreeRTOS.h"
#include <task.h>
#include <stdio.h>
#include "Task.hpp"

/**
 * @brief Create an instance of the task class.
 *
 * @param [in] taskName The name of the task to create.
 * @param [in] stackSize The size of the stack.
 * @return N/A.
 */
Task::Task(const char * taskName, uint32_t stackSize, uint8_t priority) {
	m_taskName  = taskName;
	m_stackSize = stackSize;
	m_priority  = priority;
	m_taskData  = nullptr;
	m_handle    = nullptr;
	m_coreId	= 1;
} // Task

Task::~Task() {
} // ~Task

/**
 * @brief Suspend the task for the specified milliseconds.
 *
 * @param [in] ms The delay time in milliseconds.
 * @return N/A.
 */

/* static */ void Task::delay(int ms) {
	::vTaskDelay(ms / portTICK_PERIOD_MS);
} // delay

/**
 * Static class member that actually runs the target task.
 *
 * The code here will run on the task thread.
 * @param [in] pTaskInstance The task to run.
 */
void Task::runTask(void* pTaskInstance) {
	Task* pTask = (Task*) pTaskInstance;

	printf("Printing task data\n");

    printf("%i\n", pTask->m_handle);
	printf("%s\n", pTask->m_taskData);
	printf("%s\n", pTask->m_taskName);
	printf("%i\n", pTask->m_stackSize);
	printf("%i\n", pTask->m_priority);
	printf("%i\n", pTask->m_coreId);

	
	printf(">> run: taskName=%s", pTask->m_taskName);
	pTask->run(pTask->m_taskData);
	printf("<< run: taskName=%s", pTask->m_taskName);
	pTask->stop();
} // runTask

/**
 * @brief Start an instance of the task.
 *
 * @param [in] taskData Data to be passed into the task.
 * @return N/A.
 */
void Task::start(void* taskData) {
	if (m_handle != nullptr) {
		printf("Task::start - There might be a task already running!");
	}
	m_taskData = taskData;
	
	::xTaskCreate(&runTask, m_taskName, m_stackSize, this, m_priority, &m_handle);
	vTaskCoreAffinitySet(m_handle, m_coreId);
} // start


/**
 * @brief Stop the task.
 *
 * @return N/A.
 */
void Task::stop() {
	if (m_handle == nullptr) return;
	xTaskHandle temp = m_handle;
	m_handle = nullptr;
	::vTaskDelete(temp);
} // stop

/**
 * @brief Set the stack size of the task.
 *
 * @param [in] stackSize The size of the stack for the task.
 * @return N/A.
 */
void Task::setStackSize(uint32_t stackSize) {
	m_stackSize = stackSize;
} // setStackSize

/**
 * @brief Set the priority of the task.
 *
 * @param [in] priority The priority for the task.
 * @return N/A.
 */
void Task::setPriority(uint8_t priority) {
	m_priority = priority;
} // setPriority

/**
 * @brief Set the name of the task.
 *
 * @param [in] name The name for the task.
 * @return N/A.
 */
void Task::setName(const char * name) {
	m_taskName = name;
} // setName

/**
 * @brief Get the name of the task
 * 
 * @return const char* name of the task.
 */
const char* Task::getName(){
	return m_taskName;
}

/**
 * @brief Get the TaskHandle_t of this Task.
 * 
 * @return xTaskHandle 
 */
xTaskHandle Task::getTaskHandle(){
	return m_handle;
}

/**
 * @brief Set the core number the task has to be executed on.
 * If the core number is not set, tskNO_AFFINITY will be used
 *
 * @param [in] coreId The id of the core.
 * @return N/A.
 */
void Task::setCore(BaseType_t coreId) {
	m_coreId = coreId;
}