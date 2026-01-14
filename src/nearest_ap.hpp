#pragma once

#include <nearest_ap/scheduler/bus/bus.hpp>
#include <nearest_ap/scheduler/scheduler.hpp>
#include <nearest_ap/internal/internal.hpp>

#include <nearest_ap/logger/logger.hpp>

namespace nearest_ap {

  template<typename SpawnerType >
  class Node
  {
    public:
      using Scheduler_t = Scheduler<SpawnerType>;
      using ComputePot_f = typename Scheduler_t::ComputePot_f;
      using NodeTask_f = typename Scheduler_t::NodeTaks_f;

      using VirtualId_t = Internal_t::VirtualId_t;
      using Tollercance_t = Internal_t::Tollerance_t;
      using Topology = Internal_t::Topology;

      Node() = delete;

      Node(Node&&) = default;

      Node(
          Bus_t& bus,
          SpawnerType spawner,
          Topology topology,
          const std::uint16_t current_user_index,
          ComputePot_f compute_pot_f,
          NodeTask_f leader_task_f,
          NodeTask_f slave_task_f) noexcept:
        m_internal(
            std::move(topology),
            current_user_index,
            std::move(compute_pot_f)),
        m_scheduler(std::move(spawner), bus, leader_task_f, slave_task_f, m_internal)
        {
        }

      Node(
          Bus_t& bus,
          SpawnerType spawner,
          Topology topology,
          const std::uint16_t current_user_index,
          ComputePot_f compute_pot_f,
          NodeTask_f leader_task_f,
          NodeTask_f slave_task_f,
          Tollercance_t tollerance) noexcept:
        m_internal(
            std::move(topology),
            current_user_index,
            std::move(compute_pot_f),
            tollerance),
        m_scheduler(std::move(spawner), bus, leader_task_f, slave_task_f, m_internal)
        {
        }

      Node(
          Bus_t& bus,
          SpawnerType spawner,
          Topology topology,
          const std::uint16_t current_user_index,
          ComputePot_f compute_pot_f,
          NodeTask_f leader_task_f,
          NodeTask_f slave_task_f,
          Tollercance_t tollerance,
          const Millis_t bus_task_freq,
          const Millis_t pot_task_freq,
          const Millis_t alive_task_freq) noexcept:
        m_internal(
            std::move(topology),
            current_user_index,
            std::move(compute_pot_f),
            tollerance),
        m_scheduler(
            std::move(spawner),
            bus,
            leader_task_f,
            slave_task_f,
            m_internal,
            bus_task_freq, 
            pot_task_freq,
            alive_task_freq)
        {
        }

      inline void stop(void) noexcept
      {
        m_scheduler.stop();
      }

    private:
      Internal_t m_internal;
      Scheduler_t m_scheduler;
  };
}
